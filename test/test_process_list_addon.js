'use strict';

const config    = require('../config.js');

const path      = require('path');

const chai      = require('chai');
const expect    = chai.expect;

chai.should();

describe('Process List addon', () => {
    var path_to_addon = path.join('../', config.module_file_path, 'ProcessListModule.node');

    describe('addon', () => {
        var addon;
        beforeEach(() => {
            addon = require(path_to_addon);
        });

        it('should exist', () => {
            addon.should.not.be.null;
        });

        describe('ProcessListModule class', () => {
            var process_list_module;
            beforeEach(() => {
                process_list_module = new addon.ProcessListModule();
            });

            it('should wrap and return a ProcessListModule object', () => {
                process_list_module.should.not.be.null;
            });

            describe('functions', () => {
                describe('getModuleInfo', () => {
                    it('should be a function', () => {
                        (process_list_module.getModuleInfo).should.be.a('function');
                    });
                    it('should throw if passed any parameters', () => {
                        expect(() => { process_list_module.getModuleInfo(0); }).to.throw("getModuleInfo: Expected 0 parameters and got 1.");
                    });
                    describe('module info object', () => {
                        var module_info;
                        before(() => {
                            module_info = process_list_module.getModuleInfo();
                        });

                        it('should be an object', () => {
                            expect(module_info).to.be.an('object');
                        });
                        it('should have the name of the module', () => {
                            expect(module_info.name).to.be.a('string');
                        });
                        it('should have the version number of the module', () => {
                            expect(module_info.version).to.be.a('string');
                        });
                        it('should have a description of the module', () => {
                            expect(module_info.description).to.be.a('string');
                        });
                        it('should have the Node version of the module', () => {
                            expect(module_info.node_version).to.be.a('string');
                        });
                        it('should have an array of functions (that might be empty)', () => {
                            expect(module_info.functions).to.be.an('array');
                        });
                    });
                });

                describe('getProcesses', () => {
                    it('should be a function', () => {
                        (process_list_module.getProcesses).should.be.a('function');
                    });
                    it('should throw if passed any parameters', () => {
                        expect(() => { process_list_module.getProcesses(0); }).to.throw("getProcesses: Expected 0 parameters and got 1.");
                    });

                    describe('process list array', () => {
                        var device_list;
                        before(() => {
                            device_list = process_list_module.getProcesses();
                        });

                        it('should be an array', () => {
                            expect(device_list).to.be.an('array');
                        });
                    });
                });

                describe('getProcessModules', () => {
                    it('should be a function', () => {
                        (process_list_module.getProcessModules).should.be.a('function');
                    });
                    it('should throw if passed no parameters', () => {
                        expect(() => { process_list_module.getProcessModules(); }).to.throw("getProcessModules: Expected 1 parameter and got 0.");
                    });
                    it('should throw if passed more than one parameter', () => {
                        expect(() => { process_list_module.getProcessModules(0, 1); }).to.throw("getProcessModules: Expected 1 parameter and got 2.");
                    });
                });
            });
        });
    });
});
