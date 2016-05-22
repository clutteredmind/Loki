'use strict';

var path = require('path');
var chai = require('chai');
var expect = chai.expect;

chai.should();

describe('addon tests', function () {
	var base_addon_path = '../LokiModules/x64/Release/';

	describe('ProcessListAddon', function () {
		var addon_name = 'ProcessListAddon';

		describe('addon', function () {
			var addon = null;
			beforeEach(function () {
				addon = require(path.join(base_addon_path, addon_name));
			});

			it('should exist', function () {
				addon.should.not.be.null;
			});

			describe('ProcessListAddon', function () {
				var process_list_addon = null;
				beforeEach(function () {
					process_list_addon = new addon.ProcessListAddon();
				});

				it('should return a wrapped object with "new"', function () {
					expect(process_list_addon).to.not.be.null;
				});

				describe('functions', function () {
					describe('getAddonInfo', function () {
						it('should be a function', function () {
							process_list_addon.getAddonInfo.should.be.a('function');
						});
						it('should throw if called with any parameters', function () {
							expect(function () { process_list_addon.getAddonInfo(0); }).to.throw("getAddonInfo: Expected 0 parameters and got 1.");
						});
						describe('return value', function () {
							var return_value;
							before(function () {
								return_value = process_list_addon.getAddonInfo();
							});
							it('should be an object', function () {
								return_value.should.be.an('object');
							});
							it('should have a string property called "name"', function () {
								return_value.name.should.not.be.null;
								return_value.name.should.be.a('string');
							});
							it('should have a string property called "version"', function () {
								return_value.version.should.not.be.null;
								return_value.version.should.be.a('string');
							});
							it('should have a string property called "description"', function () {
								return_value.description.should.not.be.null;
								return_value.description.should.be.a('string');
							});
							it('should have a string property called "node_version"', function () {
								return_value.node_version.should.not.be.null;
								return_value.node_version.should.be.a('string');
							});
							it('should have an object (that may be empty) that describes the addon\'s functions', function () {
								return_value.functions.should.not.be.null;
								return_value.should.be.an('object');
							});
						});
					});
					describe('getProcesses', function () {
						it('should be a function', function () {
							process_list_addon.getProcesses.should.be.a('function');
						});
						it('should return an array', function () {
							(process_list_addon.getProcesses()).should.be.an('array');
						});
						it('should throw if called with any parameters', function () {
							expect(function () { process_list_addon.getProcesses(0); }).to.throw("getProcesses: Expected 0 parameters and got 1.");
						});
					});
				});
			});
		});
	});

	describe('DeviceListAddon', function () {
		var addon_name = 'DeviceListAddon';

		describe('addon', function () {
			var addon = null;
			beforeEach(function () {
				addon = require(path.join(base_addon_path, addon_name));
			});

			it('should exist', function () {
				addon.should.not.be.null;
			});

			describe('DeviceListAddon', function () {
				var device_list_addon = null;
				beforeEach(function () {
					device_list_addon = new addon.DeviceListAddon();
				});

				it('should return a wrapped object with "new"', function () {
					expect(device_list_addon).to.not.be.null;
				});

				describe('functions', function () {
					describe('getAddonInfo', function () {
						it('should be a function', function () {
							device_list_addon.getAddonInfo.should.be.a('function');
						});
						it('should throw if called with any parameters', function () {
							expect(function () { device_list_addon.getAddonInfo(0); }).to.throw("getAddonInfo: Expected 0 parameters and got 1.");
						});
						describe('return value', function () {
							var return_value;
							before(function () {
								return_value = device_list_addon.getAddonInfo();
							});
							it('should be an object', function () {
								return_value.should.be.an('object');
							});
							it('should have a string property called "name"', function () {
								return_value.name.should.not.be.null;
								return_value.name.should.be.a('string');
							});
							it('should have a string property called "version"', function () {
								return_value.version.should.not.be.null;
								return_value.version.should.be.a('string');
							});
							it('should have a string property called "description"', function () {
								return_value.description.should.not.be.null;
								return_value.description.should.be.a('string');
							});
							it('should have a string property called "node_version"', function () {
								return_value.node_version.should.not.be.null;
								return_value.node_version.should.be.a('string');
							});
							it('should have an object (that may be empty) that describes the addon\'s functions', function () {
								return_value.functions.should.not.be.null;
								return_value.should.be.an('object');
							});
						});
					});
					describe('getDevices', function () {
						it('should be a function', function () {
							device_list_addon.getDevices.should.be.a('function');
						});
						it('should return an array', function () {
							expect(device_list_addon.getDevices()).to.be.an('array');
						});
						it('should throw if called with any parameters', function () {
							expect(function () { device_list_addon.getDevices(0); }).to.throw("getDevices: Expected 0 parameters and got 1.");
						});
					});
				});
			});
		});
	});

	describe('ScreenshotAddon', function () {
		var addon_name = 'ScreenshotAddon';

		describe('addon', function () {
			var addon = null;
			beforeEach(function () {
				addon = require(path.join(base_addon_path, addon_name));
			});

			it('should exist', function () {
				addon.should.not.be.null;
			});

			describe('ScreenshotAddon', function () {
				var screenshot_addon = null;
				beforeEach(function () {
					screenshot_addon = new addon.ScreenshotAddon();
				});

				it('should return a wrapped object with "new"', function () {
					expect(screenshot_addon).to.not.be.null;
				});

				describe('functions', function () {
					describe('getAddonInfo', function () {
						it('should be a function', function () {
							screenshot_addon.getAddonInfo.should.be.a('function');
						});
						it('should throw if called with any parameters', function () {
							expect(function () { screenshot_addon.getAddonInfo(0); }).to.throw("getAddonInfo: Expected 0 parameters and got 1.");
						});
						describe('return value', function () {
							var return_value;
							before(function () {
								return_value = screenshot_addon.getAddonInfo();
							});
							it('should be an object', function () {
								return_value.should.be.an('object');
							});
							it('should have a string property called "name"', function () {
								return_value.name.should.not.be.null;
								return_value.name.should.be.a('string');
							});
							it('should have a string property called "version"', function () {
								return_value.version.should.not.be.null;
								return_value.version.should.be.a('string');
							});
							it('should have a string property called "description"', function () {
								return_value.description.should.not.be.null;
								return_value.description.should.be.a('string');
							});
							it('should have a string property called "node_version"', function () {
								return_value.node_version.should.not.be.null;
								return_value.node_version.should.be.a('string');
							});
							it('should have an object (that may be empty) that describes the addon\'s functions', function () {
								return_value.functions.should.not.be.null;
								return_value.should.be.an('object');
							});
						});
					});
					describe('captureScreen', function () {
						it('should be a function', function () {
							screenshot_addon.captureScreen.should.be.a('function');
						});
						it('should throw if called with no parameters', function () {
							expect(function () { screenshot_addon.captureScreen(); }).to.throw("captureScreen: Expected 1 parameter and got 0.");
						});
						it('should throw if called with more than one parameter', function () {
							expect(function () { screenshot_addon.captureScreen(function () {}, false); }).to.throw("captureScreen: Expected 1 parameter and got 2.");
						});
						it('should throw if called with a parameter that is not a function', function () {
							expect(function () { screenshot_addon.captureScreen(0); }).to.throw("captureScreen: Expected parameter 0 (callback) to be a function.");
						});
					});
				});
			});
		});
	});
});
