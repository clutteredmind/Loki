'use strict';

const config    = require('../config.js');

const path      = require('path');

const chai      = require('chai');
const expect    = chai.expect;

chai.should();


describe('Screenshot addon', () => {
    var path_to_addon = path.join('../', config.module_file_path, 'ScreenshotModule.node');

    describe('addon', () => {
        var addon;
        beforeEach(() => {
            addon = require(path_to_addon);
        });

        it('should exist', () => {
            addon.should.not.be.null;
        });

        describe('ScreenshotModule class', () => {
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
