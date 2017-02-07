'use strict';

module.exports = {
    name: 'Loki',
    http_server_port: 8888,
    websocket_server_port: 8889,
    version: '1.0.0',
    static_folder_name: 'dist',
    index_file_name: 'index.html',
    handler_file_directory_name: 'message_handlers',
    // TODO: you'll want to update this to the location to which the modules are being built
    module_file_path: './LokiModules/x64/Debug/'
}
