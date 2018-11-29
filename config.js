'use strict';

module.exports = {
    name: 'Loki',
    http_server_port: 8888,
    websocket_server_port: 8889,
    version: '2.0.0',
    static_folder_name: 'loki-site/dist/loki-site',
    index_file_name: 'index.html',
    handler_file_directory_name: 'message_handlers',
    // NOTE: you'll want to update this if you want to keep the modules in a different directory
    module_file_path: './LokiModules/x64/Debug/'
}
