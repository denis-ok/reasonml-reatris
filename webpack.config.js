const HtmlWebpackPlugin = require('html-webpack-plugin')
const CleanWebpackPlugin = require("clean-webpack-plugin");

const path = require('path');

const outputDir = path.join(__dirname, "build");
const publicDir = path.join(__dirname, "public");

const isProd = process.env.NODE_ENV === 'production';

module.exports = {
  entry: './src/Index.bs.js',
  mode: isProd ? 'production' : 'development',
  output: {
    path: outputDir,
    publicPath: "/",
    filename: 'bundle.js',
  },
  module: {
    rules: [
      {
        test: /\.css$/,
        use: [
          { loader: "style-loader" },
          {
            loader: 'css-loader',
            options: {
              // modules: true,
              // localIdentName: '[local]-[hash:base64]'
            }
          },
        ]
      }
    ]
  },
  plugins: [
    new CleanWebpackPlugin(["build"]),
    new HtmlWebpackPlugin({
      template: path.join(__dirname, "src/index.html"),
    }),
  ],

  // There are two dev servers in project now: webpack-dev-server and modern webpack-serve
  // Its not necessary, so only one should remain later
  devServer: {
    contentBase: publicDir,
    historyApiFallback: true,
  },
  serve: {
    content: publicDir
  }
};
