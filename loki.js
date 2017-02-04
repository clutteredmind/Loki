// load dependencies
const fs            = require('fs');
const glob          = require('glob');
const http          = require('http');
const path          = require('path');
const express       = require('express');
const bodyParser    = require('body-parser');
const colors        = require('colors/safe');

// bring in application configuration
const config = require('./config.js');

// create the express server
const app = express();

// parsers for POST data
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));

// point static path to dist
app.use(express.static(path.join(__dirname, config.static_folder_name)));

// catch all routes and return the index file
app.get('*', (req, res) => {
    res.sendFile(path.join(__dirname, config.static_folder_name, config.index_file_name));
});

// get port from environment or config file and store in Express
const port = process.env.PORT || config.port;
app.set('port', port);

// create HTTP server
const server = http.createServer(app);

var handlers = [];
// get all the JavaScript files in the routes directory
try {
    var files = fs.readdirSync(config.handler_file_directory_name);
    for (var index in files) {
        if (files[index].toLowerCase().indexOf('.js') > -1) {
            try {
                // bring in handler and store a reference to it
                var handler = require(path.join(__dirname, config.handler_file_directory_name, files[index]));
                handlers.push(handler);
            } catch(error) {
                console.log(colors.red('Unable to require file: ' + files[index]));
            }
        }
    }
} catch(error) {
    console.log(colors.red('Failure while enumerating handler files.'));
    console.log(error);
}

// create glob path
const module_path_base = config.module_file_path + config.module_glob_string;
// get a list of modules in the specified directory
var module_files = glob.sync(module_path_base);

if(module_files.length > 0) {
    // for each module, look for a corresponding handler
    module_files.forEach((module_file) => {
        handlers.forEach((handler) => {
            if(handler.addon == undefined && module_file.indexOf(handler.module_name) >= 0) {
                try {
                    var module = require(module_file);
                    handler.addon = new module[handler.module_name]();
                } catch (error) {
                    console.log(colors.red('Unable to load module for ' + handler.module_name));
                    console.log(error);
                }
            } else if(handler.module_name == undefined) {
                // set the addon to an empty object because this module does not require one
                handler.addon = {};
            }
        });
    });
} else {
    console.log(colors.red('No module files found!'));
}

// create and open the websocket server
var WebSocketServer = require('ws').Server
const socket_server = new WebSocketServer({ port: config.websocket_server_port });

// TODO: create an API socket?

socket_server.on('connection', (socket) => {
    socket.on('message', (message) => {
        var message_object = JSON.parse(message);
        // handle system message
        if(message_object.category == 'system') {
            // register and unregister messages will come in here and can be used for tool use tracking
            switch(message_object.action) {
                case 'register':
                break;
                case 'unregister':
                break;
            }
        } else {
            // look for a handler
            handlers.forEach((handler) => {
                if(handler.message_category == message_object.category) {
                    if(handler.addon != undefined) {
                        try {
                            var result = handler.handle_message(handler.addon, message_object);
                            socket.send(JSON.stringify({
                                category: message_object.category,
                                action: message_object.action,
                                data: result
                            }),
                            (error) => {
                                if(error) {
                                    console.log(colors.red('Unable to send socket message.'));
                                    console.log(error);
                                }
                            });
                        } catch(error) {
                            var error_message = 'Failure in the ' + message_object.category + ' module\'s process_message method.'
                            // attempt to send the error to anyone listening
                            socket.send(JSON.stringify({
                                category: message_object.category,
                                action: 'error',
                                data: error_message
                            }),
                            (error) => {
                                if(error) {
                                    console.log(colors.red('Unable to send socket message.'));
                                    console.log(error);
                                }
                            });
                            console.log(colors.red(error_message));
                            console.log(error);
                        }
                    } else {
                        socket.send(JSON.stringify({
                            category: message_object.category,
                            action: 'error',
                            data: 'Could not handle "' + message_object.category + '" message. Module not loaded.'
                        }),
                        (error) => {
                            if(error) {
                                console.log(colors.red('Unable to send socket message.'));
                                console.log(error);
                            }
                        });
                    }
                }
            });
        }
    });

    socket.on('close', () => {
        // TODO: do something when socket closes?
    });
});

// listen on provided port, on all network interfaces.
server.listen(port, () => {
    // report PID for developers
    console.log(colors.green("Process ID is: " + process.pid));
    // display port, just as a reminder
    console.log(colors.green('Server running on port: ' + port));
});
