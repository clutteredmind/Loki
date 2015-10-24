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
				var process_list_addon;
				beforeEach(function () {
					process_list_addon = new addon.ProcessListAddon();
				});

				it('should return a wrapped object with "new"', function () {
					process_list_addon.should.be.an.Object;
				});

				describe('functions', function () {
					describe('getProcesses', function () {
						it('should be a function', function () {
							process_list_addon.getProcesses.should.be.a.Function;
						});
						it('should return an array', function () {
							(process_list_addon.getProcesses()).should.be.an.Array;
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
					device_list_addon.should.be.an.Object;
				});

				describe('functions', function () {
					describe('getDevices', function () {
						it('should be a function', function () {
							device_list_addon.getDevices.should.be.a.Function;
						});
						it('should return an array', function () {
							(device_list_addon.getDevices()).should.be.an.Array;
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
					screenshot_addon.should.be.an.Object;
				});

				describe('functions', function () {
					describe('captureScreen', function () {
						it('should be a function', function () {
							screenshot_addon.captureScreen.should.be.a.Function;
						});
						it('should throw an exception if called with no arguments', function () {
							expect(screenshot_addon.captureScreen.bind(screenshot_addon)).to.throw("Invalid parameter count. Expected one function parameter.");
						});
						it('should throw an exception if called with an argument that is not a function', function () {
							expect(screenshot_addon.captureScreen.bind(screenshot_addon, true)).to.throw("The parameter must be a function.");
						});
						it('should return "undefined" when called with a callback', function () {
							expect(screenshot_addon.captureScreen(function () { })).to.be.Undefined;
						});
					});
				});
			});
		});
	});
});
