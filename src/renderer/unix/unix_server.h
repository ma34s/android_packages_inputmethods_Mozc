// Copyright 2010-2015, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef MOZC_RENDERER_UNIX_UNIX_SERVER_H_
#define MOZC_RENDERER_UNIX_UNIX_SERVER_H_

#include <memory>

#include "base/mutex.h"
#include "renderer/renderer_server.h"
#include "renderer/unix/gtk_wrapper_interface.h"

namespace mozc {
namespace renderer {
namespace gtk {

class WindowManager;

class UnixServer : public RendererServer {
 public:
  struct MozcWatchSource {
    GSource source;
    GPollFD poll_fd;
    UnixServer *unix_server;
  };
  // UnixServer takes arguments' ownership.
  explicit UnixServer(GtkWrapperInterface *gtk);
  ~UnixServer();

  virtual void AsyncHide();
  virtual void AsyncQuit();
  virtual bool AsyncExecCommand(string *proto_message);
  virtual int StartMessageLoop();

  virtual bool Render();

  void OpenPipe();

 private:
  string message_;
  Mutex mutex_;
  std::unique_ptr<GtkWrapperInterface> gtk_;

  // Following pipe is used to communicate IPC recieving thread and
  // rendering(gtk-main) thread. The gtk-main loop polls following pipe and IPC
  // recieving thread writes small data to notify gtk-main thread to update when
  // new packet is arrived.
  int pipefd_[2];

  DISALLOW_COPY_AND_ASSIGN(UnixServer);
};

}  // namespace gtk
}  // namespace renderer
}  // namespace mozc
#endif  // MOZC_RENDERER_UNIX_UNIX_SERVER_H_