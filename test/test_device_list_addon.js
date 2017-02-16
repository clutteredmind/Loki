'use strict';

const config    = require('../config.js');

const path      = require('path');

const chai      = require('chai');
const expect    = chai.expect;

chai.should();

describe('Device List addon', () => {
    var path_to_addon = path.join('../', config.module_file_path, 'DeviceListModule.node');

    describe('addon', () => {
        var addon;
        beforeEach(() => {
            addon = require(path_to_addon);
        });

        it('should exist', () => {
            addon.should.not.be.null;
        });

        describe('DeviceListModule class', () => {
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
