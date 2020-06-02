open Types;
open Belt;

[@bs.module "./GridOverlay.module.css"]
external styles: Js.t({..}) = "default";

module TitleScreen = {
  [@react.component]
  let make = (~clickStart) =>
    <>
      <h2 className=styles##titleHeading> {React.string("REATRIS")} </h2>
      <div className=styles##btn onClick=clickStart>
        <p> {React.string("Start Game")} </p>
      </div>
    </>;
};

module CounterScreen = {
  [@react.component]
  let make = (~countdownCounter: countdownCounter) =>
    <p className=styles##number>
      {React.string(Int.toString(countdownCounter))}
    </p>;
};

module GameoverScreen = {
  [@react.component]
  let make = (~clickStart) =>
    <div className=styles##btn onClick=clickStart>
      <p> {React.string("Play Again?")} </p>
    </div>;
};

[@react.component]
let make = (~screen: Screen.t, ~countdownCounter, ~clickStart) =>
  <div className=styles##gridOverlay>
    {switch (screen) {
     | Title => <TitleScreen clickStart />
     | Counter => <CounterScreen countdownCounter />
     | Game => React.null
     | Gameover => <GameoverScreen clickStart />
     }}
  </div>;
