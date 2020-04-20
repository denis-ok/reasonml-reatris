babelOptions = {
  presets: [
    [
      "@babel/preset-env",
      {
        targets: {
          node: 12,
        },
      },
    ],
  ],
};

module.exports = require("babel-jest").createTransformer(babelOptions);
