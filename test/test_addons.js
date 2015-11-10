'use strict';

var path = require('path');
var chai = require('chai');
var expect = chai.expect;

chai.should();

describe('addon tests', function () {
	var base_addon_path = '../LokiAddons/x64/Debug/';

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
						it('should throw an exception if called with no arguments', function () {
							expect(function () { screenshot_addon.captureScreen(); }).to.throw("Invalid parameter count. Expected one function parameter.");
						});
						it('should throw an exception if called with an argument that is not a function', function () {
							expect(function () { screenshot_addon.captureScreen(0); }).to.throw("The parameter must be a function.");
						});
					});
				});
			});
		});
	});
});
