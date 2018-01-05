type job;

let kill: job => unit;

/**
 * Run an async command until it exits.
 */
let run: job => unit;

/**
 * Run multiple async commands in parallel until they all exit.
 */
let runAll: list(job) => unit;
