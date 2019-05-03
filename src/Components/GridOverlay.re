[@bs.config {jsx: 3}];

module RR = ReasonReact;
open Types;

[%bs.raw {|require('./gridOverlay.css')|}];

module TitleScreen = {
  [@react.component]
  let make = (~clickStart) => {
    <div className="grid-overlay">
      <h2 className="title-heading"> {RR.string("REATRIS")} </h2>
      <div className="btn" onClick=clickStart>
        <p> {RR.string("Start Game")} </p>
      </div>
    </div>;
  };
};

module CounterScreen = {
  [@react.component]
  let make = (~countdownCounter: countdownCounter) => {
    <div className="grid-overlay">
      <p className="number">
        {RR.string(string_of_int(countdownCounter))}
      </p>
    </div>;
  };
};

module GameoverScreen = {
  [@react.component]
  let make = (~clickStart) => {
    <div className="grid-overlay">
      <div className="btn" onClick=clickStart>
        <p> {RR.string("Play Again?")} </p>
      </div>
    </div>;
  };
};

[@react.component]
let make = (~screen, ~countdownCounter, ~clickStart) => {
  switch (screen) {
  | Title => <TitleScreen clickStart />
  | Counter => <CounterScreen countdownCounter />
  | Game => RR.null
  | Gameover => <GameoverScreen clickStart />
  };
};

module Jsx2 = {
  let component = ReasonReact.statelessComponent("GridOverlay");
  let make = (~screen, ~countdownCounter, ~clickStart, children) =>
    ReasonReactCompat.wrapReactForReasonReact(
      make,
      makeProps(~screen, ~countdownCounter, ~clickStart, ()),
      children,
    );
};
