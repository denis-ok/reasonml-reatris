open Types;

let styles = [%raw {|require('./GridOverlay.module.css')|}];

module TitleScreen = {
  [@react.component]
  let make = (~clickStart) => {
    <div className=styles##gridOverlay>
      <h2 className=styles##titleHeading> {React.string("REATRIS")} </h2>
      <div className=styles##btn onClick=clickStart>
        <p> {React.string("Start Game")} </p>
      </div>
    </div>;
  };
};

module CounterScreen = {
  [@react.component]
  let make = (~countdownCounter: countdownCounter) => {
    <div className=styles##gridOverlay>
      <p className=styles##number>
        {React.string(string_of_int(countdownCounter))}
      </p>
    </div>;
  };
};

module GameoverScreen = {
  [@react.component]
  let make = (~clickStart) => {
    <div className=styles##gridOverlay>
      <div className=styles##btn onClick=clickStart>
        <p> {React.string("Play Again?")} </p>
      </div>
    </div>;
  };
};

[@react.component]
let make = (~screen, ~countdownCounter, ~clickStart) => {
  switch (screen) {
  | Title => <TitleScreen clickStart />
  | Counter => <CounterScreen countdownCounter />
  | Game => React.null
  | Gameover => <GameoverScreen clickStart />
  };
};
