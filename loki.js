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
                console.log(error);
            }
        }
    }
} catch(error) {
    console.log(colors.red('Failure while enumerating handler files.'));
    console.log(error);
}

// get a list of modules in the specified directory
var module_files = finder.from(config.module_file_path).findFiles('*.node');

var addons = [];
module_files.forEach((module_file) => {
    var module = require(module_file);
    for(var key in module) {
        addons.push(new module[key]());
    }
});

// bring in custom handler functions
var custom_handler_map = require('./message_handlers/custom.handler.js');

// create and open the websocket server
var WebSocketServer = require('ws').Server
const socket_server = new WebSocketServer({ port: config.websocket_server_port });

// TODO: create an API socket?

socket_server.on('connection', (socket) => {
    socket.on('message', (message) => {
        var message_object = JSON.parse(message);
        // handle system message
        if(message_object.category == 'system') {
            // register and unregister messages will come in here and can be used for user tracking
            switch(message_object.action) {
                case 'register':
                    break;
                case 'unregister':
                    break;
            }
        } else {
            // see if the function can be called
            var handled = false;
            // check to see if there is a custom handler for this action
            var func = custom_handler_map.get(message_object.action);
            if(func) {
                socket.send(JSON.stringify({
                    category: message_object.category,
                    action: message_object.action,
                    data: func(addons, message_object.parameters)
                }),
                (error) => {
                    if(error) {
                        console.log(colors.red('Unable to send socket message.'));
                        console.log(error);
                    }
                });
                handled = true;
            }
            addons.forEach((addon) => {
                for(var key in addon) {
                    if(key == message_object.action) {
                        if(!handled) {
                            socket.send(JSON.stringify({
                                category: message_object.category,
                                action: message_object.action,
                                data: addon[message_object.action]()
                            }),
                            (error) => {
                                if(error) {
                                    console.log(colors.red('Unable to send socket message.'));
                                    console.log('The error was: ' + error);
                                }
                            });
                            handled = true;
                        }
                    }
                }
            });
            if(!handled) {
                socket.send(JSON.stringify({
                    category: message_object.category,
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
