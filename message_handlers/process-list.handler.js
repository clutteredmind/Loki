'use strict';

var process_list_custom_functions = [];

process_list_custom_functions.push({
    specifier: 'ProcessListModule',
    name: 'getProcessModules',
    function: (component, addons, socket, parameters) => {
        var result;
        addons.forEach((addon) => {
            addon.functions.forEach((function_name) => {
                if(function_name == 'getProcessModules') {
                    result = addon.addon.getProcessModules(parameters[0]);
                }
            });
        });
        return result;
    }
});

module.exports = process_list_custom_functions;
