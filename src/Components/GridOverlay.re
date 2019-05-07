open Types;

[%bs.raw {|require('./gridOverlay.css')|}];

module TitleScreen = {
  [@react.component]
  let make = (~clickStart) => {
    <div className="grid-overlay">
      <h2 className="title-heading"> {React.string("REATRIS")} </h2>
      <div className="btn" onClick=clickStart>
        <p> {React.string("Start Game")} </p>
      </div>
    </div>;
  };
};

module CounterScreen = {
  [@react.component]
  let make = (~countdownCounter: countdownCounter) => {
    <div className="grid-overlay">
      <p className="number">
        {React.string(string_of_int(countdownCounter))}
      </p>
    </div>;
  };
};

module GameoverScreen = {
  [@react.component]
  let make = (~clickStart) => {
    <div className="grid-overlay">
      <div className="btn" onClick=clickStart>
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
