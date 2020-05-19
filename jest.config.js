module.exports = {
  testEnvironment: "jsdom",
  // testURL: "http://localhost/",
  verbose: true,
  testRegex: "\\_test\\.bs\\.js",
  transform: {
    "^.+\\.jsx?$": `<rootDir>/babel.test.config.js`,
  },
  // babel-jest ignores transforming of node_modules dir by default
  // it gives error below:
  // import * as List from "bs-platform/lib/es6/list.js";
  // ^^^^^^
  // SyntaxError: Cannot use import statement outside a module
  // Avoiding it with transformIgnorePatterns option
  transformIgnorePatterns: ["<rootDir>/public/"],
};
