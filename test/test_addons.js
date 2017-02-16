'use strict';

const config    = require('../config.js');

const path      = require('path');

const chai      = require('chai');
const expect    = chai.expect;

chai.should();

describe('Addon tests', () => {
    describe('DeviceListModule', () => {
        var path_to_addon = path.join('../', config.module_file_path, 'DeviceListModule.node');

        describe('addon', () => {
            var addon;
            beforeEach(() => {
                addon = require(path_to_addon);
            });

            it('should exist', () => {
                addon.should.not.be.null;
            });

            describe('DeviceListModule', () => {
                var device_list_module;
                beforeEach(() => {
                    device_list_module = new addon.DeviceListModule();
                });

                it('should wrap and return a DeviceListModule object', () => {
                    device_list_module.should.not.be.null;
                });

                describe('functions', () => {
                    describe('getModuleInfo', () => {
                        it('should be a function', () => {
                            (device_list_module.getModuleInfo).should.be.a('function');
                        });
                        it('should throw if passed any parameters', () => {
                            expect(() => { device_list_module.getModuleInfo(0); }).to.throw("getModuleInfo: Expected 0 parameters and got 1.");
                        });
                        describe('module info object', () => {
                            var module_info;
                            before(() => {
                                module_info = device_list_module.getModuleInfo();
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

                    describe('getDevices', () => {
                        it('should be a function', () => {
                            (device_list_module.getDevices).should.be.a('function');
                        });
                        it('should throw if passed any parameters', () => {
                            expect(() => { device_list_module.getDevices(0); }).to.throw("getDevices: Expected 0 parameters and got 1.");
                        });

                        describe('device list array', () => {
                            var device_list;
                            before(() => {
                                device_list = device_list_module.getDevices();
                            });

                            it('should be an array', () => {
                                expect(device_list).to.be.an('array');
                            });
                        });
                    });
                });
            });
        });
    });

    describe('ProcessListModule', () => {
        var path_to_addon = path.join('../', config.module_file_path, 'ProcessListModule.node');

        describe('addon', () => {
            var addon;
            beforeEach(() => {
                addon = require(path_to_addon);
            });

            it('should exist', () => {
                addon.should.not.be.null;
            });

            describe('ProcessListModule', () => {
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

    describe('ScreenshotModule', () => {
        var path_to_addon = path.join('../', config.module_file_path, 'ScreenshotModule.node');

        describe('addon', () => {
            var addon;
            beforeEach(() => {
                addon = require(path_to_addon);
            });

            it('should exist', () => {
                addon.should.not.be.null;
            });

            describe('ScreenshotModule', () => {
                var screenshot_module;
                beforeEach(() => {
                    screenshot_module = new addon.ScreenshotModule();
                });

                it('should wrap and return a ScreenshotModule object', () => {
                    screenshot_module.should.not.be.null;
                });

                describe('functions', () => {
                    describe('getModuleInfo', () => {
                        it('should be a function', () => {
                            (screenshot_module.getModuleInfo).should.be.a('function');
                        });
                        it('should throw if passed any parameters', () => {
                            expect(() => { screenshot_module.getModuleInfo(0); }).to.throw("getModuleInfo: Expected 0 parameters and got 1.");
                        });
                        describe('module info object', () => {
                            var module_info;
                            before(() => {
                                module_info = screenshot_module.getModuleInfo();
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

                    describe('captureScreen', () => {
                        it('should be a function', () => {
                            (screenshot_module.captureScreen).should.be.a('function');
                        });
                        it('should throw if passed no parameters', () => {
                            expect(() => { screenshot_module.captureScreen(); }).to.throw("captureScreen: Expected 1 parameter and got 0.");
                        });
                        it('should throw if passed more than one parameter', () => {
                            expect(() => { screenshot_module.captureScreen(0, 1); }).to.throw("captureScreen: Expected 1 parameter and got 2.");
                        });
                    });
                });
            });
        });
    });

    describe('SystemInformationModule', () => {
        var path_to_addon = path.join('../', config.module_file_path, 'SystemInformationModule.node');

        describe('addon', () => {
            var addon;
            beforeEach(() => {
                addon = require(path_to_addon);
            });

            it('should exist', () => {
                addon.should.not.be.null;
            });

            describe('SystemInformationModule', () => {
                var system_information_module;
                beforeEach(() => {
                    system_information_module = new addon.SystemInformationModule();
                });

                it('should wrap and return a ScreenshotModule object', () => {
                    system_information_module.should.not.be.null;
                });

                describe('functions', () => {
                    describe('getModuleInfo', () => {
                        it('should be a function', () => {
                            (system_information_module.getModuleInfo).should.be.a('function');
                        });
                        it('should throw if passed any parameters', () => {
                            expect(() => { system_information_module.getModuleInfo(0); }).to.throw("getModuleInfo: Expected 0 parameters and got 1.");
                        });
                        describe('module info object', () => {
                            var module_info;
                            before(() => {
                                module_info = system_information_module.getModuleInfo();
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

                    describe('getMemoryInformation', () => {
                        it('should be a function', () => {
                            (system_information_module.getMemoryInformation).should.be.a('function');
                        });
                        it('should throw if passed a parameter', () => {
                            expect(() => { system_information_module.getMemoryInformation(0); }).to.throw("getMemoryInformation: Expected 0 parameters and got 1.");
                        });

                        describe('memory information object', () => {
                            var memory_info;
                            before(() => {
                                memory_info = system_information_module.getMemoryInformation();
                            });

                            it('should be an object', () => {
                                expect(memory_info).to.be.an('object');
                            });
                        });
                    });

                    describe('getCpuInformation', () => {
                        it('should be a function', () => {
                            (system_information_module.getCpuInformation).should.be.a('function');
                        });
                        it('should throw if passed a parameter', () => {
                            expect(() => { system_information_module.getCpuInformation(0); }).to.throw("getCpuInformation: Expected 0 parameters and got 1.");
                        });

                        describe('CPU information object', () => {
                            var cpu_info;
                            before(() => {
                                cpu_info = system_information_module.getCpuInformation();
                            });

                            it('should be an object', () => {
                                expect(cpu_info).to.be.an('object');
                            });
                        });
                    });
                });
            });
        });
    });
});
