open Types;

[%bs.raw {|require('./stats.css')|}];

module StatsRow = {
  [@react.component]
  let make = (~name: string, ~value: int) => {
    <div className="statsRow">
      <p className="statName"> {React.string(name)} </p>
      <p className="statValue"> {React.string(string_of_int(value))} </p>
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
       React.null;
     }}
  </div>;
};
