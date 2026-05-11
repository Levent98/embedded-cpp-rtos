#ifndef MODBUSSERVICE_HPP
#define MODBUSSERVICE_HPP

#include <cstdint>
#include <cstddef>
#include "UartDriver.hpp"
#include "ErrorManager.hpp"

extern "C" {
#include "modbus_rtu.h"
#include "sensortypes.h"
}

class ModbusService {
public:
	  ~ModbusService() = default;

    ModbusService(UartDriver& uart, ErrorManager& errors);
     
    ModbusService(const ModbusService&) = delete;
    ModbusService& operator=(const ModbusService&) = delete;
    
    ModbusService(ModbusService&&) = delete;
    ModbusService& operator=(ModbusService&&) = delete;

    void initialize(uint8_t slaveAddress);
    void process();

    void setHoldingRegister(uint16_t reg, uint16_t value);
    [[nodiscard]] uint16_t holdingRegister(uint16_t reg) const;

    void setInputRegister(uint16_t reg, uint16_t value);
    [[nodiscard]] uint16_t inputRegister(uint16_t reg) const;

    void updateRegisters(uint16_t startReg, const uint16_t* data, uint16_t count);

    void syncAll();
    void sensorUpdate();
    void updateLiveInputs();

    uint8_t queueFrame(const uint8_t* data, uint16_t len);
    uint8_t dequeueFrame(frame_item_t* frame);
    [[nodiscard]] const frame_item_t* peekFrame() const;
    void popFrame();

    [[nodiscard]] uint8_t queueCount() const;
    [[nodiscard]] uint8_t isQueueFull() const;
    [[nodiscard]] uint8_t isQueueEmpty() const;
    void clearQueue();
    [[nodiscard]] uint32_t overflowCount() const;

    uint8_t processFrame(const frame_item_t* frame);
    void sendErrorResponse(uint8_t function, uint8_t errorCode);

private:
    static const uint16_t InputRegSize = HOLDING_REG_SIZE;

    uint16_t holdingRegs_[HOLDING_REG_SIZE];
    uint16_t inputRegs_[InputRegSize];
    frame_queue_t frameQueue_;

    uint8_t rxScratch_[UartDriver::BufferSize];
    uint8_t responseScratch_[256U];
    frame_item_t frameScratch_;
    SensorStruct sensorScratch_;
    AlarmStruct alarmScratch_[4U];
    uint16_t alarmThresholdScratch_[4U];

    UartDriver& uart_;
    ErrorManager& errors_;
    uint8_t slaveAddress_;

    int16_t tempVal_;
    int16_t humVal_;

    static uint16_t crc16(const uint8_t* buffer, uint16_t len);

    uint8_t processReadHoldingRegisters(const frame_item_t* frame);
    uint8_t processReadInputRegisters(const frame_item_t* frame);
    uint8_t processWriteSingleRegister(const frame_item_t* frame);
    uint8_t processWriteMultipleRegisters(const frame_item_t* frame);

    void setHoldingRegisterInternal(uint16_t reg, uint16_t value);
    [[nodiscard]] uint16_t getHoldingRegisterInternal(uint16_t reg) const;
    void setInputRegisterInternal(uint16_t reg, uint16_t value);
    [[nodiscard]] uint16_t getInputRegisterInternal(uint16_t reg) const;

    static uint8_t isWritable(uint16_t addr);
    uint8_t runCommand(uint16_t startAddr, uint16_t qty);
    void updateDiagnosticRegisters();

    uint8_t saveBaseSettings(const uint16_t* data, uint8_t len);
    uint8_t saveSensorSettings(const uint16_t* data, uint8_t len);
};

#endif