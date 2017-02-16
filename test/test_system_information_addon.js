'use strict';

const config    = require('../config.js');

const path      = require('path');

const chai      = require('chai');
const expect    = chai.expect;

chai.should();

describe('System Information addon', () => {
    var path_to_addon = path.join('../', config.module_file_path, 'SystemInformationModule.node');

    describe('addon', () => {
        var addon;
        beforeEach(() => {
            addon = require(path_to_addon);
        });

        it('should exist', () => {
            addon.should.not.be.null;
        });

        describe('SystemInformationModule class', () => {
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
