/*
 * The MySensors Arduino library handles the wireless radio link and protocol
 * between your home built sensors/actuators and HA controller of choice.
 * The sensors forms a self healing radio network with optional repeaters. Each
 * repeater and gateway builds a routing tables in EEPROM which keeps track of the
 * network topology allowing messages to be routed to nodes.
 *
 * Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
 * Copyright (C) 2013-2015 Sensnology AB
 * Full contributor list: https://github.com/mysensors/Arduino/graphs/contributors
 *
 * Documentation: http://www.mysensors.org
 * Support Forum: http://forum.mysensors.org
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 */

/**
 * @file MyMessage.h
 *
 * @brief API and type declarations for MySensors messages
 * @defgroup MyMessagegrp MyMessage
 * @{
 *
 * @brief Here you can find all message types used by the MySensors protocol as well as macros for
 * parsing and manipulating messages.
 */
#ifndef MyMessage_h
#define MyMessage_h

#ifdef __cplusplus
	#ifdef ARDUINO
		#include <Arduino.h>
	#endif
#endif

#include <stdint.h>

#define PROTOCOL_VERSION 2    //!< The version of the protocol
#define MAX_MESSAGE_LENGTH 32 //!< The maximum size of a message (including header)
#define HEADER_SIZE 7         //!< The size of the header
#define MAX_PAYLOAD (MAX_MESSAGE_LENGTH - HEADER_SIZE) //!< The maximum size of a payload depends on #MAX_MESSAGE_LENGTH and #HEADER_SIZE

/// @brief The command field (message-type) defines the overall properties of a message
typedef enum {
	C_PRESENTATION			= 0,	//!< Sent by a node when they present attached sensors. This is usually done in presentation() at startup.
	C_SET					= 1,	//!< This message is sent from or to a sensor when a sensor value should be updated.
	C_REQ					= 2,	//!< Requests a variable value (usually from an actuator destined for controller).
	C_INTERNAL				= 3,	//!< Internal MySensors messages (also include common messages provided/generated by the library).
	C_STREAM				= 4		//!< For firmware and other larger chunks of data that need to be divided into pieces.
} mysensor_command;

/// @brief Type of sensor (used when presenting sensors)
typedef enum {
	S_DOOR					= 0,	//!< Door sensor, V_TRIPPED, V_ARMED
	S_MOTION				= 1,	//!< Motion sensor, V_TRIPPED, V_ARMED 
	S_SMOKE					= 2,	//!< Smoke sensor, V_TRIPPED, V_ARMED
	S_BINARY				= 3,	//!< Binary light or relay, V_STATUS, V_WATT
	S_LIGHT					= 3,	//!< \deprecated Same as S_BINARY, **** DEPRECATED, DO NOT USE ****
	S_DIMMER				= 4,	//!< Dimmable light or fan device, V_STATUS (on/off), V_PERCENTAGE (dimmer level 0-100), V_WATT 
	S_COVER					= 5,	//!< Blinds or window cover, V_UP, V_DOWN, V_STOP, V_PERCENTAGE (open/close to a percentage)
	S_TEMP					= 6,	//!< Temperature sensor, V_TEMP
	S_HUM					= 7,	//!< Humidity sensor, V_HUM
	S_BARO					= 8,	//!< Barometer sensor, V_PRESSURE, V_FORECAST
	S_WIND					= 9,	//!< Wind sensor, V_WIND, V_GUST
	S_RAIN					= 10,	//!< Rain sensor, V_RAIN, V_RAINRATE
	S_UV					= 11,	//!< Uv sensor, V_UV
	S_WEIGHT				= 12,	//!< Personal scale sensor, V_WEIGHT, V_IMPEDANCE
	S_POWER					= 13,	//!< Power meter, V_WATT, V_KWH, V_VAR, V_VA, V_POWER_FACTOR
	S_HEATER				= 14,	//!< Header device, V_HVAC_SETPOINT_HEAT, V_HVAC_FLOW_STATE, V_TEMP
	S_DISTANCE				= 15,	//!< Distance sensor, V_DISTANCE
	S_LIGHT_LEVEL			= 16,	//!< Light level sensor, V_LIGHT_LEVEL (uncalibrated in percentage),  V_LEVEL (light level in lux)
	S_ARDUINO_NODE			= 17,	//!< Used (internally) for presenting a non-repeating Arduino node
	S_ARDUINO_REPEATER_NODE	= 18,	//!< Used (internally) for presenting a repeating Arduino node 
	S_LOCK					= 19,	//!< Lock device, V_LOCK_STATUS
	S_IR					= 20,	//!< IR device, V_IR_SEND, V_IR_RECEIVE
	S_WATER					= 21,	//!< Water meter, V_FLOW, V_VOLUME
	S_AIR_QUALITY			= 22,	//!< Air quality sensor, V_LEVEL
	S_CUSTOM				= 23,	//!< Custom sensor 
	S_DUST					= 24,	//!< Dust sensor, V_LEVEL
	S_SCENE_CONTROLLER		= 25,	//!< Scene controller device, V_SCENE_ON, V_SCENE_OFF. 
	S_RGB_LIGHT				= 26,	//!< RGB light. Send color component data using V_RGB. Also supports V_WATT 
	S_RGBW_LIGHT			= 27,	//!< RGB light with an additional White component. Send data using V_RGBW. Also supports V_WATT
	S_COLOR_SENSOR			= 28,	//!< Color sensor, send color information using V_RGB
	S_HVAC					= 29,	//!< Thermostat/HVAC device. V_HVAC_SETPOINT_HEAT, V_HVAC_SETPOINT_COLD, V_HVAC_FLOW_STATE, V_HVAC_FLOW_MODE, V_TEMP
	S_MULTIMETER			= 30,	//!< Multimeter device, V_VOLTAGE, V_CURRENT, V_IMPEDANCE 
	S_SPRINKLER				= 31,	//!< Sprinkler, V_STATUS (turn on/off), V_TRIPPED (if fire detecting device)
	S_WATER_LEAK			= 32,	//!< Water leak sensor, V_TRIPPED, V_ARMED
	S_SOUND					= 33,	//!< Sound sensor, V_TRIPPED, V_ARMED, V_LEVEL (sound level in dB)
	S_VIBRATION				= 34,	//!< Vibration sensor, V_TRIPPED, V_ARMED, V_LEVEL (vibration in Hz)
	S_MOISTURE				= 35,	//!< Moisture sensor, V_TRIPPED, V_ARMED, V_LEVEL (water content or moisture in percentage?) 
	S_INFO					= 36,	//!< LCD text device / Simple information device on controller, V_TEXT
	S_GAS					= 37,	//!< Gas meter, V_FLOW, V_VOLUME
	S_GPS					= 38,	//!< GPS Sensor, V_POSITION
	S_WATER_QUALITY			= 39	//!< V_TEMP, V_PH, V_ORP, V_EC, V_STATUS 
} mysensor_sensor;

/// @brief Type of sensor data (for set/req/ack messages)
typedef enum {
	V_TEMP					= 0,	//!< S_TEMP. Temperature S_TEMP, S_HEATER, S_HVAC
	V_HUM					= 1,	//!< S_HUM. Humidity
	V_STATUS				= 2,	//!< S_BINARY, S_DIMMER, S_SPRINKLER, S_HVAC, S_HEATER. Used for setting/reporting binary (on/off) status. 1=on, 0=off  
	V_LIGHT					= 2,	//!< \deprecated Same as V_STATUS, **** DEPRECATED, DO NOT USE ****
	V_PERCENTAGE			= 3,	//!< S_DIMMER. Used for sending a percentage value 0-100 (%). 
	V_DIMMER				= 3,	//!< \deprecated Same as V_PERCENTAGE, **** DEPRECATED, DO NOT USE **** 
	V_PRESSURE				= 4,	//!< S_BARO. Atmospheric Pressure
	V_FORECAST				= 5,	//!< S_BARO. Whether forecast. string of "stable", "sunny", "cloudy", "unstable", "thunderstorm" or "unknown"
	V_RAIN					= 6,	//!< S_RAIN. Amount of rain
	V_RAINRATE				= 7,	//!< S_RAIN. Rate of rain
	V_WIND					= 8,	//!< S_WIND. Wind speed
	V_GUST					= 9,	//!< S_WIND. Gust
	V_DIRECTION				= 10,	//!< S_WIND. Wind direction 0-360 (degrees)
	V_UV					= 11,	//!< S_UV. UV light level
	V_WEIGHT				= 12,	//!< S_WEIGHT. Weight(for scales etc)
	V_DISTANCE				= 13,	//!< S_DISTANCE. Distance
	V_IMPEDANCE				= 14,	//!< S_MULTIMETER, S_WEIGHT. Impedance value
	V_ARMED					= 15,	//!< S_DOOR, S_MOTION, S_SMOKE, S_SPRINKLER. Armed status of a security sensor. 1 = Armed, 0 = Bypassed
	V_TRIPPED				= 16,	//!< S_DOOR, S_MOTION, S_SMOKE, S_SPRINKLER, S_WATER_LEAK, S_SOUND, S_VIBRATION, S_MOISTURE. Tripped status of a security sensor. 1 = Tripped, 0
	V_WATT					= 17,	//!< S_POWER, S_BINARY, S_DIMMER, S_RGB_LIGHT, S_RGBW_LIGHT. Watt value for power meters
	V_KWH					= 18,	//!< S_POWER. Accumulated number of KWH for a power meter
	V_SCENE_ON				= 19,	//!< S_SCENE_CONTROLLER. Turn on a scene
	V_SCENE_OFF				= 20,	//!< S_SCENE_CONTROLLER. Turn of a scene
	V_HVAC_FLOW_STATE		= 21,	//!< S_HEATER, S_HVAC. HVAC flow state ("Off", "HeatOn", "CoolOn", or "AutoChangeOver") 
	V_HEATER				= 21,	//!< \deprecated Same as V_HVAC_FLOW_STATE, **** DEPRECATED, DO NOT USE ****
	V_HVAC_SPEED			= 22,	//!< S_HVAC, S_HEATER. HVAC/Heater fan speed ("Min", "Normal", "Max", "Auto") 
	V_LIGHT_LEVEL			= 23,	//!< S_LIGHT_LEVEL. Uncalibrated light level. 0-100%. Use V_LEVEL for light level in lux
	V_VAR1					= 24,	//!< VAR1 
	V_VAR2					= 25,	//!< VAR2 
	V_VAR3					= 26,	//!< VAR3
	V_VAR4					= 27,	//!< VAR4
	V_VAR5					= 28,	//!< VAR5
	V_UP					= 29,	//!< S_COVER. Window covering. Up
	V_DOWN					= 30,	//!< S_COVER. Window covering. Down
	V_STOP					= 31,	//!< S_COVER. Window covering. Stop
	V_IR_SEND				= 32,	//!< S_IR. Send out an IR-command
	V_IR_RECEIVE			= 33,	//!< S_IR. This message contains a received IR-command
	V_FLOW					= 34,	//!< S_WATER. Flow of water (in meter)
	V_VOLUME				= 35,	//!< S_WATER. Water volume
	V_LOCK_STATUS			= 36,	//!< S_LOCK. Set or get lock status. 1=Locked, 0=Unlocked
	V_LEVEL					= 37,	//!< S_DUST, S_AIR_QUALITY, S_SOUND (dB), S_VIBRATION (hz), S_LIGHT_LEVEL (lux)
	V_VOLTAGE				= 38,	//!< S_MULTIMETER 
	V_CURRENT				= 39,	//!< S_MULTIMETER
	V_RGB					= 40,	//!< S_RGB_LIGHT, S_COLOR_SENSOR. Sent as ASCII hex: RRGGBB (RR=red, GG=green, BB=blue component)
	V_RGBW					= 41,	//!< S_RGBW_LIGHT. Sent as ASCII hex: RRGGBBWW (WW=white component)
	V_ID					= 42,	//!< Used for sending in sensors hardware ids (i.e. OneWire DS1820b). 
	V_UNIT_PREFIX			= 43,	//!< Allows sensors to send in a string representing the unit prefix to be displayed in GUI, not parsed by controller! E.g. cm, m, km, inch.
	V_HVAC_SETPOINT_COOL	= 44,	//!< S_HVAC. HVAC cool setpoint (Integer between 0-100)
	V_HVAC_SETPOINT_HEAT	= 45,	//!< S_HEATER, S_HVAC. HVAC/Heater setpoint (Integer between 0-100)
	V_HVAC_FLOW_MODE		= 46,	//!< S_HVAC. Flow mode for HVAC ("Auto", "ContinuousOn", "PeriodicOn")
	V_TEXT					= 47,	//!< S_INFO. Text message to display on LCD or controller device
	V_CUSTOM				= 48,	//!< Custom messages used for controller/inter node specific commands, preferably using S_CUSTOM device type.
	V_POSITION				= 49,	//!< GPS position and altitude. Payload: latitude;longitude;altitude(m). E.g. "55.722526;13.017972;18"
	V_IR_RECORD				= 50,	//!< Record IR codes S_IR for playback
	V_PH					= 51,	//!< S_WATER_QUALITY, water PH
    V_ORP					= 52,	//!< S_WATER_QUALITY, water ORP : redox potential in mV
    V_EC					= 53,	//!< S_WATER_QUALITY, water electric conductivity μS/cm (microSiemens/cm)
    V_VAR					= 54,	//!< S_POWER, Reactive power: volt-ampere reactive (var)
    V_VA					= 55,	//!< S_POWER, Apparent power: volt-ampere (VA)
    V_POWER_FACTOR			= 56,	//!< S_POWER, Ratio of real power to apparent power: floating point value in the range [-1,..,1]
} mysensor_data;


/// @brief Type of internal messages (for internal messages)
typedef enum {
	I_BATTERY_LEVEL			= 0,	//!< Battery level
	I_TIME					= 1,	//!< Time
	I_VERSION				= 2,	//!< Version
	I_ID_REQUEST			= 3,	//!< ID request
	I_ID_RESPONSE			= 4,	//!< ID response
	I_INCLUSION_MODE		= 5,	//!< Inclusion mode
	I_CONFIG				= 6,	//!< Config
	I_FIND_PARENT			= 7,	//!< Find parent
	I_FIND_PARENT_RESPONSE	= 8,	//!< Find parent response
	I_LOG_MESSAGE			= 9,	//!< Log message
	I_CHILDREN				= 10,	//!< Children
	I_SKETCH_NAME			= 11,	//!< Sketch name
	I_SKETCH_VERSION		= 12,	//!< Sketch version
	I_REBOOT				= 13,	//!< Reboot request
	I_GATEWAY_READY			= 14,	//!< Gateway ready
	I_SIGNING_PRESENTATION	= 15,	//!< Provides signing related preferences (first byte is preference version)
	I_NONCE_REQUEST			= 16,	//!< Request for a nonce
	I_NONCE_RESPONSE		= 17,	//!< Payload is nonce data
	I_HEARTBEAT				= 18,	//!< Heartbeat request
	I_PRESENTATION			= 19,	//!< Presentation message
	I_DISCOVER				= 20,	//!< Discover request
	I_DISCOVER_RESPONSE		= 21,	//!< Discover response
	I_HEARTBEAT_RESPONSE	= 22,	//!< Heartbeat response
	I_LOCKED				= 23,	//!< Node is locked (reason in string-payload)
	I_PING					= 24,	//!< Ping sent to node, payload incremental hop counter
	I_PONG					= 25,	//!< In return to ping, sent back to sender, payload incremental hop counter
	I_REGISTRATION_REQUEST	= 26,	//!< Register request to GW
	I_REGISTRATION_RESPONSE	= 27,	//!< Register response from GW
	I_DEBUG					= 28	//!< Debug message
} mysensor_internal;


/// @brief Type of data stream  (for streamed message)
typedef enum {
	ST_FIRMWARE_CONFIG_REQUEST	= 0,	//!< Request new FW, payload contains current FW details
	ST_FIRMWARE_CONFIG_RESPONSE	= 1,	//!< New FW details to initiate OTA FW update
	ST_FIRMWARE_REQUEST			= 2,	//!< Request FW block
	ST_FIRMWARE_RESPONSE		= 3,	//!< Response FW block
	ST_SOUND					= 4,	//!< Sound
	ST_IMAGE					= 5		//!< Image
} mysensor_stream;

/// @brief Type of payload
typedef enum {
	P_STRING				= 0,	//!< Payload type is string
	P_BYTE					= 1,	//!< Payload type is byte
	P_INT16					= 2,	//!< Payload type is INT16
	P_UINT16				= 3,	//!< Payload type is UINT16
	P_LONG32				= 4,	//!< Payload type is INT32
	P_ULONG32				= 5,	//!< Payload type is UINT32
	P_CUSTOM				= 6,	//!< Payload type is binary
	P_FLOAT32				= 7		//!< Payload type is float32
} mysensor_payload;



#ifndef BIT
#define BIT(n)                  ( 1<<(n) ) //!< Bit indexing macro
#endif
#define BIT_MASK(len)           ( BIT(len)-1 ) //!< Create a bitmask of length 'len'
#define BF_MASK(start, len)     ( BIT_MASK(len)<<(start) ) //!< Create a bitfield mask of length starting at bit 'start'

#define BF_PREP(x, start, len)  ( ((x)&BIT_MASK(len)) << (start) ) //!< Prepare a bitmask for insertion or combining
#define BF_GET(y, start, len)   ( ((y)>>(start)) & BIT_MASK(len) ) //!< Extract a bitfield of length 'len' starting at bit 'start' from 'y'
#define BF_SET(y, x, start, len)    ( y= ((y) &~ BF_MASK(start, len)) | BF_PREP(x, start, len) ) //!< Insert a new bitfield value 'x' into 'y'

// Getters/setters for special bit fields in header
#define mSetVersion(_msg,_version) BF_SET(_msg.version_length, _version, 0, 2) //!< Set version field
#define mGetVersion(_msg) ((uint8_t)BF_GET(_msg.version_length, 0, 2)) //!< Get version field

#define mSetSigned(_msg,_signed) BF_SET(_msg.version_length, _signed, 2, 1) //!< Set signed field
#define mGetSigned(_msg) ((bool)BF_GET(_msg.version_length, 2, 1)) //!< Get versignedsion field

#define mSetLength(_msg,_length) BF_SET(_msg.version_length, _length, 3, 5) //!< Set length field
#define mGetLength(_msg) ((uint8_t)BF_GET(_msg.version_length, 3, 5)) //!< Get length field

#define mSetCommand(_msg,_command) BF_SET(_msg.command_ack_payload, _command, 0, 3) //!< Set command field
#define mGetCommand(_msg) ((uint8_t)BF_GET(_msg.command_ack_payload, 0, 3)) //!< Get command field

#define mSetRequestAck(_msg,_rack) BF_SET(_msg.command_ack_payload, _rack, 3, 1) //!< Set ack-request field
#define mGetRequestAck(_msg) ((bool)BF_GET(_msg.command_ack_payload, 3, 1)) //!< Get  ack-request field

#define mSetAck(_msg,_ackMsg) BF_SET(_msg.command_ack_payload, _ackMsg, 4, 1) //!< Set ack field
#define mGetAck(_msg) ((bool)BF_GET(_msg.command_ack_payload, 4, 1)) //!< Get ack field

#define mSetPayloadType(_msg, _pt) BF_SET(_msg.command_ack_payload, _pt, 5, 3) //!< Set payload type field
#define mGetPayloadType(_msg) ((uint8_t)BF_GET(_msg.command_ack_payload, 5, 3)) //!< Get payload type field


// internal access for special fields
#define miGetCommand() ((uint8_t)BF_GET(command_ack_payload, 0, 3)) //!< Internal getter for command field

#define miSetLength(_length) BF_SET(version_length, _length, 3, 5) //!< Internal setter for length field
#define miGetLength() ((uint8_t)BF_GET(version_length, 3, 5)) //!< Internal getter for length field

#define miSetRequestAck(_rack) BF_SET(command_ack_payload, _rack, 3, 1) //!< Internal setter for ack-request field
#define miGetRequestAck() ((bool)BF_GET(command_ack_payload, 3, 1)) //!< Internal getter for ack-request field

#define miSetAck(_ack) BF_SET(command_ack_payload, _ack, 4, 1) //!< Internal setter for ack field
#define miGetAck() ((bool)BF_GET(command_ack_payload, 4, 1)) //!< Internal getter for ack field

#define miSetPayloadType(_pt) BF_SET(command_ack_payload, _pt, 5, 3) //!< Internal setter for payload type field
#define miGetPayloadType() (uint8_t)BF_GET(command_ack_payload, 5, 3) //!< Internal getter for payload type field


#if !DOXYGEN
#ifdef __cplusplus
class MyMessage
{
private:
	char* getCustomString(char *buffer) const;

public:
	// Constructors
	MyMessage();

	MyMessage(uint8_t sensor, uint8_t type);

	char i2h(uint8_t i) const;

	/**
	 * If payload is something else than P_STRING you can have the payload value converted
	 * into string representation by supplying a buffer with the minimum size of
	 * 2*MAX_PAYLOAD+1. This is to be able to fit hex-conversion of a full binary payload.
	 */
	char* getStream(char *buffer) const;
	char* getString(char *buffer) const;
	const char* getString() const;
	void* getCustom() const;
	bool getBool() const;
	uint8_t getByte() const;
	float getFloat() const;
	int16_t getInt() const;
	uint16_t getUInt() const;
	int32_t getLong() const;
	uint32_t getULong() const;

	// Getter for command type
	uint8_t getCommand() const;

	// Getter for ack-flag. True if this is an ack message.
	bool isAck() const;

	// Setters for building message "on the fly"
	MyMessage& setType(uint8_t type);
	MyMessage& setSensor(uint8_t sensor);
	MyMessage& setDestination(uint8_t destination);

	// Setters for payload
	MyMessage& set(void* payload, uint8_t length);
	MyMessage& set(const char* value);
	MyMessage& set(float value, uint8_t decimals);
	MyMessage& set(bool value);
	MyMessage& set(uint8_t value);
	MyMessage& set(uint32_t value);
	MyMessage& set(int32_t value);
	MyMessage& set(uint16_t value);
	MyMessage& set(int16_t value);

#else

typedef union {
struct
{

#endif
	uint8_t last;            	 // 8 bit - Id of last node this message passed
	uint8_t sender;          	 // 8 bit - Id of sender node (origin)
	uint8_t destination;     	 // 8 bit - Id of destination node

	uint8_t version_length;		 // 2 bit - Protocol version
			                     // 1 bit - Signed flag
			                     // 5 bit - Length of payload
	uint8_t command_ack_payload; // 3 bit - Command type
	                             // 1 bit - Request an ack - Indicator that receiver should send an ack back.
								 // 1 bit - Is ack messsage - Indicator that this is the actual ack message.
	                             // 3 bit - Payload data type
	uint8_t type;            	 // 8 bit - Type varies depending on command
	uint8_t sensor;          	 // 8 bit - Id of sensor that this message concerns.

	// Each message can transfer a payload. We add one extra byte for string
	// terminator \0 to be "printable" this is not transferred OTA
	// This union is used to simplify the construction of the binary data types transferred.
	union {
		uint8_t bValue;
		uint16_t uiValue;
		int16_t iValue;
		uint32_t ulValue;
		int32_t lValue;
		struct { // Float messages
			float fValue;
			uint8_t fPrecision;   // Number of decimals when serializing
		};
		struct {  // Presentation messages
			uint8_t version; 	  // Library version
   		    uint8_t sensorType;   // Sensor type hint for controller, see table above
		};
		char data[MAX_PAYLOAD + 1];
	} __attribute__((packed));
#ifdef __cplusplus
} __attribute__((packed));
#else
};
uint8_t array[HEADER_SIZE + MAX_PAYLOAD + 1];	
} __attribute__((packed)) MyMessage;
#endif
#endif

#endif
/** @}*/
