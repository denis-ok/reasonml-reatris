open Types;

let styles = [%bs.raw {|require('./Stats.module.css')|}];

module StatsRow = {
  [@react.component]
  let make = (~name: string, ~value: int) => {
    <div className=styles##statsRow>
      <p> {React.string(name)} </p>
      <p> {React.string(string_of_int(value))} </p>
    </div>;
  };
};

[@react.component]
let make = (~stats: stats, ~started) => {
  <div className=styles##sideContainer>
    {if (started) {
       <div className=styles##stats>
         <StatsRow name="Score" value={stats.score} />
         <StatsRow name="Lines" value={stats.lines} />
         <StatsRow name="Level" value={stats.level} />
       </div>;
     } else {
       React.null;
     }}
  </div>;
};
