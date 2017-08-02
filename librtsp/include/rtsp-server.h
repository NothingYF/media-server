#ifndef _rtsp_server_h_
#define _rtsp_server_h_

#include "rtsp-header-transport.h"
#include <stdint.h>
#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct rtsp_server_t rtsp_server_t;
typedef struct rtsp_session_t rtsp_session_t;

struct rtsp_handler_t
{
	/// RTSP DESCRIBE request
	/// @param[in] ptr user-defined parameter
	/// @param[in] rtsp request handle
	/// @param[in] uri request uri
	/// Required: MUST call rtsp_server_reply_describe once
	void (*describe)(void* ptr, rtsp_session_t* rtsp, const char* uri);

	/// RTSP SETUP request
	/// @param[in] ptr user-defined parameter
	/// @param[in] rtsp request handle
	/// @param[in] uri request uri
	/// @param[in] transport RTSP Transport header
	/// Required: MUST call rtsp_server_reply_setup once
	void (*setup)(void* ptr, rtsp_session_t* rtsp, const char* uri, const char* session, const struct rtsp_header_transport_t transports[], size_t num);

	/// RTSP PLAY request
	/// @param[in] ptr user-defined parameter
	/// @param[in] rtsp request handle
	/// @param[in] session RTSP Session
	/// @param[in] npt request begin time, NULL if don't have Range parameter, 0 represent now
	/// @param[in] scale request scale, NULL if don't have Scale parameter
	/// Required: MUST call rtsp_server_reply_play once
	void (*play)(void* ptr, rtsp_session_t* rtsp, const char* uri, const char* session, const int64_t *npt, const double *scale);

	/// RTSP PAUSE request
	/// @param[in] ptr user-defined parameter
	/// @param[in] rtsp request handle
	/// @param[in] session RTSP Session
	/// @param[in] npt request pause time, NULL if don't have Range parameter
	/// Required: MUST call rtsp_server_reply_pause once
	void (*pause)(void* ptr, rtsp_session_t* rtsp, const char* uri, const char* session, const int64_t *npt);

	/// RTSP TEARDOWN request
	/// @param[in] ptr user-defined parameter
	/// @param[in] rtsp request handle
	/// @param[in] session RTSP Session
	/// @param[in] uri request uri
	/// Required: MUST call rtsp_server_reply_teardown once
	void (*teardown)(void* ptr, rtsp_session_t* rtsp, const char* uri, const char* session);
};

/// start rtsp server
/// @param[in] ip bind ip address, NULL for any network interface
/// @param[in] port bind port, default 554
/// @return NULL-error, other-rtsp server instance
rtsp_server_t* rtsp_server_create(const char* ip, int port, struct rtsp_handler_t* handler, void* ptr);

/// stop rtsp server
/// @param[in] server rtsp server instance
/// @return 0-ok, other-error code
int rtsp_server_destroy(rtsp_server_t* server);

/// RTSP DESCRIBE reply
/// @param[in] rtsp request handle
/// @param[in] code RTSP status-code(200-OK, 301-Move Permanently, ...)
/// @param[in] sdp RTSP SDP
void rtsp_server_reply_describe(rtsp_session_t* rtsp, int code, const char* sdp);

/// RTSP SETUP reply
/// @param[in] rtsp request handle
/// @param[in] code RTSP status-code(200-OK, 301-Move Permanently, ...)
/// @param[in] session RTSP Session parameter
/// @param[in] transport RTSP Transport parameter
void rtsp_server_reply_setup(rtsp_session_t* rtsp, int code, const char* session, const char* transport);

/// RTSP PLAY reply
/// @param[in] rtsp request handle
/// @param[in] code RTSP status-code(200-OK, 301-Move Permanently, ...)
/// @param[in] nptstart Range start time(ms) [optional]
/// @param[in] nptend Range end time(ms) [optional]
/// @param[in] rtpinfo RTP-info [optional] e.g. url=rtsp://foo.com/bar.avi/streamid=0;seq=45102,url=rtsp://foo.com/bar.avi/streamid=1;seq=30211
void rtsp_server_reply_play(rtsp_session_t* rtsp, int code, const int64_t *nptstart, const int64_t *nptend, const char* rtpinfo);

/// RTSP PAUSE reply
/// @param[in] rtsp request handle
/// @param[in] code RTSP status-code(200-OK, 301-Move Permanently, ...)
void rtsp_server_reply_pause(rtsp_session_t* rtsp, int code);

/// RTSP PAUSE reply
/// @param[in] rtsp request handle
/// @param[in] code RTSP status-code(200-OK, 301-Move Permanently, ...)
void rtsp_server_reply_teardown(rtsp_session_t* rtsp, int code);

/// find RTSP header
/// @param[in] rtsp request handle
/// @param[in] name header name
/// @return header value, NULL if not found.
/// Required: call in rtsp_handler_t callback only
const char* rtsp_server_get_header(rtsp_session_t* rtsp, const char* name);

/// Get client ip/port
/// @param[in] rtsp request handle
/// @param[out] ip client bind ip
/// @param[out] port client bind port
/// @return 0-ok, other-error
int rtsp_server_get_client(rtsp_session_t* rtsp, char ip[65], unsigned short *port);

#if defined(__cplusplus)
}
#endif
#endif /* !_rtsp_server_h_ */
