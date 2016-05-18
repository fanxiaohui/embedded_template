set(SOURCES
    ${SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/MQTTClient-C/src/MQTTClient.c
    ${CMAKE_CURRENT_SOURCE_DIR}/MQTTPacket/src/MQTTPacket.c
    ${CMAKE_CURRENT_SOURCE_DIR}/MQTTPacket/src/MQTTConnectClient.c
    ${CMAKE_CURRENT_SOURCE_DIR}/MQTTPacket/src/MQTTConnectServer.c
    ${CMAKE_CURRENT_SOURCE_DIR}/MQTTPacket/src/MQTTDeserializePublish.c
    ${CMAKE_CURRENT_SOURCE_DIR}/MQTTPacket/src/MQTTSerializePublish.c
    ${CMAKE_CURRENT_SOURCE_DIR}/MQTTPacket/src/MQTTSubscribeClient.c
    ${CMAKE_CURRENT_SOURCE_DIR}/MQTTPacket/src/MQTTSubscribeServer.c
    ${CMAKE_CURRENT_SOURCE_DIR}/MQTTPacket/src/MQTTUnsubscribeClient.c
    ${CMAKE_CURRENT_SOURCE_DIR}/MQTTPacket/src/MQTTUnsubscribeServer.c
    PARENT_SCOPE
)

