[@bs.config {jsx: 3}];

[%bs.raw {|require('./App.css')|}];

[@react.component]
let make = () =>
  <div className="App"> <div className="App-header" /> <Game /> </div>;
