let styles = [%bs.raw {|require('./App.module.css')|}];

[@react.component]
let make = () =>
  <div className=styles##app>
    <div className=styles##appHeader />
    <Game />
  </div>;
