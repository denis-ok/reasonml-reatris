[@bs.config {jsx: 3}];

module RR = ReasonReact;
open Types;

[%bs.raw {|require('./stats.css')|}];

module StatsRow = {
  [@react.component]
  let make = (~name: string, ~value: int) => {
    <div className="statsRow">
      <p className="statName"> {RR.string(name)} </p>
      <p className="statValue"> {RR.string(string_of_int(value))} </p>
    </div>;
  };
};

[@react.component]
let make = (~stats: stats, ~started) => {
  <div className="sideContainer">
    {if (started) {
       <div className="stats">
         <StatsRow name="Score" value={stats.score} />
         <StatsRow name="Lines" value={stats.lines} />
         <StatsRow name="Level" value={stats.level} />
       </div>;
     } else {
       RR.null;
     }}
  </div>;
};

module Jsx2 = {
  let component = ReasonReact.statelessComponent("Stats");
  let make = (~stats: stats, ~started, children) =>
    ReasonReactCompat.wrapReactForReasonReact(
      make,
      makeProps(~stats: stats, ~started, ()),
      children,
    );
};
