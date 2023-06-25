/*
 * File: jest.config.ts
 * Created Date: Sunday, June 25th 2023
 * Author: Aaron Hall
 * -----
 * Last Modified: Sunday, 25th June 2023 12:54:27 pm
 * Modified By: Aaron Hall
 * -----
 * Copyright (c) 2020 - 2023
 */

export default {
  clearMocks: true,
  collectCoverage: true,
  coverageDirectory: '../coverage',
  coverageProvider: 'v8',
  rootDir: './build'
}
