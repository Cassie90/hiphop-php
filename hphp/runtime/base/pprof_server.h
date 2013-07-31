/*
   +----------------------------------------------------------------------+
   | HipHop for PHP                                                       |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010-2013 Facebook, Inc. (http://www.facebook.com)     |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
*/

#ifndef incl_HPHP_PPROF_HEAP_SERVER_H_
#define incl_HPHP_PPROF_HEAP_SERVER_H_

#include "hphp/runtime/server/server.h"
#include "hphp/runtime/base/profile_dump.h"

namespace HPHP {

struct HeapProfileRequestHandler : public RequestHandler {
  explicit HeapProfileRequestHandler(int timeout)
    : RequestHandler(timeout) {}

  virtual ~HeapProfileRequestHandler() {}
  virtual void handleRequest(Transport *transport);

};

DECLARE_BOOST_TYPES(HeapProfileServer);
struct HeapProfileServer {

  HeapProfileServer() :
    m_server(ServerFactoryRegistry::createServer(
      RuntimeOption::ServerType,
      RuntimeOption::ServerIP,
      (uint16_t)4327,          // port
      1                        // thread count
    )
  ) {
    m_server->setRequestHandlerFactory<HeapProfileRequestHandler>(30);
    m_server->start();
  }

  virtual ~HeapProfileServer() {
    m_server->stop();
    m_server->waitForEnd();
  }

  static HeapProfileServerPtr Server;

private:
  const ServerPtr m_server;
};

}

#endif // incl_HPHP_PPROF_HEAP_SERVER_H_
