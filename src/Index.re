[@bs.module "normalize.css"] external _normalize: Js.t({..}) = "default";
[@bs.module "./index.css"] external _styles: Js.t({..}) = "default";

ReactDOMRe.renderToElementWithId(<App />, "root");
