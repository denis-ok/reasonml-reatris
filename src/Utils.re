module Dom = {
  let getKeyName = Webapi.Dom.KeyboardEvent.key;
  let isKeyRepeated = Webapi.Dom.KeyboardEvent.repeat;
};

let clearIntervalId = (id: Types.intervalId) =>
  switch (id^) {
  | Some(id) => Js.Global.clearInterval(id)
  | None => ()
  };
