module Dom = {
  let getKeyName = Webapi.Dom.KeyboardEvent.key;
  let isKeyRepeated = Webapi.Dom.KeyboardEvent.repeat;

  let addKeyboardListeners = (~keyDownHandler, ~keyUpHandler, document) => {
    Webapi.Dom.Document.addKeyDownEventListener(keyDownHandler, document);
    Webapi.Dom.Document.addKeyUpEventListener(keyUpHandler, document);
  };

  let removeKeyboardListeners = (~keyDownHandler, ~keyUpHandler, document) => {
    Webapi.Dom.Document.removeKeyDownEventListener(keyDownHandler, document);
    Webapi.Dom.Document.removeKeyUpEventListener(keyUpHandler, document);
  };
};

let clearIntervalId = (id: Types.intervalId) =>
  switch (id^) {
  | Some(id) => Js.Global.clearInterval(id)
  | None => ()
  };
