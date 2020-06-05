open Belt;

[@bs.module "./GridOverlay.module.css"]
external styles: Js.t({..}) = "default";

module TitleScreen = {
  [@react.component]
  let make = (~onClickStart) =>
    <>
      <h2 className=styles##titleHeading> {React.string("REATRIS")} </h2>
      <div className=styles##btn onClick=onClickStart>
        <p> {React.string("Start Game")} </p>
      </div>
    </>;
};

module CounterScreen = {
  [@react.component]
  let make = (~countdownCounter: int) =>
    <p className=styles##number>
      {React.string(Int.toString(countdownCounter))}
    </p>;
};

module GameoverScreen = {
  [@react.component]
  let make = (~onClickStart) =>
    <div className=styles##btn onClick=onClickStart>
      <p> {React.string("Play Again?")} </p>
    </div>;
};

[@react.component]
let make =
    (
      ~screen: Types.Screen.t,
      ~countdownCounter: int,
      ~onClickStart: unit => unit,
    ) => {
  let onClickStart = event => {
    ReactEvent.Mouse.preventDefault(event);
    ReactEvent.Mouse.stopPropagation(event);
    onClickStart();
  };

  <div className=styles##gridOverlay>
    {switch (screen) {
     | Title => <TitleScreen onClickStart />
     | Counter => <CounterScreen countdownCounter />
     | Game => React.null
     | Gameover => <GameoverScreen onClickStart />
     }}
  </div>;
};
