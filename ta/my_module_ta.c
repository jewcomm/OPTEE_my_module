#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>
#include <tee_isocket.h>
#include <tee_tcpsocket.h>
#include <tee_udpsocket.h>

#include <my_module_ta.h>
#include <string.h>

#define __maybe_unused  __attribute__((unused))

//extern TEE_iSocket * const TEE_tcpSocket;
//extern TEE_iSocket * const TEE_udpSocket;

void resultAndErrorToDMSG(uint32_t result, uint32_t error);

/*
 * Called when the instance of the TA is created. This is the first call in
 * the TA.
 */
TEE_Result TA_CreateEntryPoint(void)
{
	DMSG("has been called");

	return TEE_SUCCESS;
}

/*
 * Called when the instance of the TA is destroyed if the TA has not
 * crashed or panicked. This is the last call in the TA.
 */
void TA_DestroyEntryPoint(void)
{
	DMSG("has been called");
}

/*
 * Called when a new session is opened to the TA. *sess_ctx can be updated
 * with a value to be able to identify this session in subsequent calls to the
 * TA. In this function you will normally do the global initialization for the
 * TA.
 */
TEE_Result TA_OpenSessionEntryPoint(uint32_t param_types,
		TEE_Param __maybe_unused params[4],
		void __maybe_unused **sess_ctx)
{
	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE);

	DMSG("has been called");

	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;

	/* Unused parameters */
	(void)&params;
	(void)&sess_ctx;

	/*
	 * The DMSG() macro is non-standard, TEE Internal API doesn't
	 * specify any means to logging from a TA.
	 */
	IMSG("Hello World!\n");

	/* If return value != TEE_SUCCESS the session will not be created. */
	return TEE_SUCCESS;
}

/*
 * Called when a session is closed, sess_ctx hold the value that was
 * assigned by TA_OpenSessionEntryPoint().
 */
void TA_CloseSessionEntryPoint(void __maybe_unused *sess_ctx)
{
	(void)&sess_ctx; /* Unused parameter */
	IMSG("Goodbye!\n");
}

static TEE_Result write_hw_sh(uint32_t param_types,
	TEE_Param params[4])
{
		uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE);

	DMSG("has been called write hello world module");

	TEE_Result res = TEE_ERROR_GENERIC;

	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;

	char msg[] = "Hello World!\0";

	TEE_iSocketHandle handle;
	//TEE_iSocket *socket;
	TEE_udpSocket_Setup setup = {};
	setup.ipVersion = TEE_IP_VERSION_4;
	setup.server_port = 3000;
	char * addr = "192.168.1.70";
	setup.server_addr = addr;
	
	uint32_t protError;

	//socket = TEE_udpSocket;

	int counter = 10;

	char * buff = "22";
	uint32_t len = 3;

	res = TEE_tcpSocket->open(&handle, &setup, &protError);
	DMSG("Try connection return");
	resultAndErrorToDMSG(res, protError);
	DMSG("");

	res = TEE_tcpSocket->send(&handle, (const void *)buff, &len, 0);
	DMSG("Try send return");
	resultAndErrorToDMSG(res, protError);
	DMSG("");
/*	TEE_tcpSocket_Setup setup = { };
	setup.ipVersion = TEE_IP_VERSION_4;
	setup.server_port = 3000;
	setup.server_addr = "127.0.0.1";
	uint32_t protError;

	socket = TEE_tcpSocket;
	res = socket->open(&handle, &setup, &protError);
	DMSG("Try connection return: %i	protocol error: %i", res, protError);*/

	return res;
}
/*
 * Called when a TA is invoked. sess_ctx hold that value that was
 * assigned by TA_OpenSessionEntryPoint(). The rest of the paramters
 * comes from normal world.
 */
TEE_Result TA_InvokeCommandEntryPoint(void __maybe_unused *sess_ctx,
			uint32_t cmd_id,
			uint32_t param_types, TEE_Param params[4])
{
	(void)&sess_ctx; /* Unused parameter */

	switch (cmd_id) {
	case TA_HELLO_WORLD_CMD_WRITE_HW:
		return write_hw_sh(param_types, params);
	default:
		return TEE_ERROR_BAD_PARAMETERS;
	}
}

void resultAndErrorToDMSG(uint32_t result, uint32_t error){
	switch (result)
	{
	case TEE_SUCCESS:
		DMSG("Return TEE_SUCCESS");
		break;
	case TEE_ERROR_BAD_PARAMETERS:
		DMSG("Return TEE_ERROR_BAD_PARAMETERS");
		break;
	case TEE_ERROR_COMMUNICATION:
		DMSG("Return TEE_ERROR_COMMUNICATION");
		break;
	case TEE_ISOCKET_ERROR_TIMEOUT:
		DMSG("Return TEE_ISOCKET_ERROR_TIMEOUT");
		break;
	default:
		DMSG("Another return: %i", result);
		break;
	}

	switch (error)
	{
	case TEE_ISOCKET_ERROR_PROTOCOL:
		DMSG("TEE_ISOCKET_ERROR_PROTOCOL");
		break;
	case TEE_ISOCKET_ERROR_REMOTE_CLOSED:
		DMSG("TEE_ISOCKET_ERROR_REMOTE_CLOSED");
		break;
	case TEE_ISOCKET_ERROR_TIMEOUT:
		DMSG("TEE_ISOCKET_ERROR_TIMEOUT");
		break;
	case TEE_ISOCKET_ERROR_OUT_OF_RESOURCES:
		DMSG("TEE_ISOCKET_ERROR_OUT_OF_RESOURCES");
		break;		
	case TEE_ISOCKET_ERROR_LARGE_BUFFER:
		DMSG("TEE_ISOCKET_ERROR_LARGE_BUFFER");
		break;	
	case TEE_ISOCKET_WARNING_PROTOCOL:
		DMSG("TEE_ISOCKET_WARNING_PROTOCOL");
		break;
	case TEE_ISOCKET_ERROR_HOSTNAME:
		DMSG("TEE_ISOCKET_ERROR_HOSTNAME");
		break;
	default:
		DMSG("Another return: %i", error);
		break;
	}

}