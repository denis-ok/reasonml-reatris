open Types;

Random.init(int_of_float(Js.Date.now()));

let getRandomElemIndex = arr => Random.int(Array.length(arr));

let blockT =
[|
[|X, X, X|],
[|O, X, O|],
|];

let blockBox =
[|
[|X, X|],
[|X, X|],
|];

let blockL =
[|
[|X, O|],
[|X, O|],
[|X, X|],
|];

let blockZ =
[|
[|O, X, X|],
[|X, X, O|],
|];

let blockI =
[|
[|X, X, X, X|]
|];

let blockColl = [|blockT, blockBox, blockL, blockZ, blockI|];

let getRandomBlock = () => blockColl[getRandomElemIndex(blockColl)];
