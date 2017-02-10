// load dependencies
const fs            = require('fs');
const http          = require('http');
const path          = require('path');
const express       = require('express');
const finder        = require('fs-finder');
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
const port = process.env.PORT || config.http_server_port;
app.set('port', port);

// create HTTP server
const server = http.createServer(app);

// get a list of modules in the specified directory
var module_files = finder.from(config.module_file_path).findFiles('*.node');

var addons = [];
module_files.forEach((module_file) => {
    try {
        var module = require(module_file);
        for(var key in module) {
            var new_object = {};
            new_object.name = key;
            new_object.addon = new module[key]();
            new_object.functions = [];
            for(var key in new_object.addon) {
                new_object.functions.push(key);
            }
            addons.push(new_object);
        }
    } catch(error) {
        console.log(colors.red('There was a problem with ' + module_file));
        console.log(error);
    }
});

// no addons is potentially a problem
if(addons.length < 1) {
    console.log(colors.red('No module files found!'));
} else {
    console.log(colors.green('Loaded ' + addons.length + ' addon' + (addons.length > 1 ? 's' : '') + '.'));
}

// bring in custom handler files
var handler_files = finder.from('./message_handlers').findFiles('*.handler.js');
// get handler objects
var handlers = [];
handler_files.forEach((handler_file) => {
    try {
        handlers.push(require(handler_file));
    } catch(error) {
        console.log(colors.red('There was a problem with ' + handler_file));
        console.log(error);
    }
});

// no handlers is potentially a problem
if(handlers.length < 1) {
    console.log(colors.red('No handler files found!'));
} else {
    console.log(colors.green('Loaded ' + handlers.length + ' custom handler' + (handlers.length > 1 ? 's' : '') + '.'));
}

// create and open the websocket server
var WebSocketServer = require('ws').Server
const socket_server = new WebSocketServer({ port: config.websocket_server_port });

// TODO: create an API socket?

socket_server.on('connection', (socket) => {
    socket.on('message', (message) => {
        var message_object = JSON.parse(message);
        // handle system message
        if(message_object.component == 'system') {
            // register and unregister messages will come in here and can be used for user tracking
            switch(message_object.action) {
                case 'register':
                    break;
                case 'unregister':
                    break;
            }
        } else {
            var handled = false;
            try {
                // look for a custom handler for this message
                handlers.forEach((handler_functions) => {
                    handler_functions.forEach((handler_function) => {
                        if((handler_function.specifier == message_object.specifier || !message_object.specifier) && handler_function.name == message_object.action && !handled) {
                            socket.send(JSON.stringify({
                                component: message_object.component,
                                action: message_object.action,
                                data: handler_function.function(message_object.component, addons, socket, message_object.parameters)
                            }));
                            handled = true;
                        }
                    });
                });
            } catch(error) {
                handled = true;
            }
            // look for a default handler if the message was not handled above
            if(!handled) {
                try {
                    addons.forEach((addon) => {
                        addon.functions.forEach((function_name) => {
                            if((addon.name == message_object.specifier || !message_object.specifier) && function_name == message_object.action && !handled) {
                                socket.send(JSON.stringify({
                                    component: message_object.component,
                                    action: message_object.action,
                                    data: addon.addon[message_object.action]()
                                }),
                                (error) => {
                                    if(error) {
                                        console.log(colors.red('Unable to send socket message.'));
                                        console.log(error);
                                    }
                                });
                                handled = true;
                            }
                        });
                    });
                } catch(error) {
                    socket.send(JSON.stringify({
                        component: message_object.component,
                        action: 'error',
                        data: error.message
                    }),
                    (error) => {
                        if(error) {
                            console.log(colors.red('Unable to send socket message.'));
                            console.log(error);
                        }
                    });
                    // something went wrong, but we don't want to try to handle this again below.
                    handled = true;
                }
            }
            // if it's still unhandled, there was a problem
            if(!handled) {
                socket.send(JSON.stringify({
                    component: message_object.component,
                    action: 'error',
                    data: 'No handler for ' + message_object.action
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
