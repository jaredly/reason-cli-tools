/**
 * Get the output of a command, in lines.
 */
let readCommand: (~cmd: string, ~onOut: string => unit=?, unit) => (list(string), bool);

let pollableCommand: (~cmd: string, ~onOut: string => unit) => (unit => unit, unit => unit);

let keepAlive:
  (
    ~cmd: string,
    ~onOut: string => unit=?,
    ~onErr: string => unit=?,
    ~onStart: unit => unit=?,
    ~checkTime: float=?,
    unit
  ) =>
  (unit => unit, unit => unit);