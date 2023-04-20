self.Module = {
    locateFile: function (s) {
        return s;
    },
    // Add this function
    onRuntimeInitialized: function () {
        var query = get();
        postMessage(query);
    },
};

self.importScripts("astro.js");

self.data = {};

// to pass data from the main JS file
self.onmessage = function (messageEvent) {
    self.data = messageEvent.data; // save the data
};

// gets executed when everything is ready.
self.get = function () {
    var calc = self.Module.ccall(
        "get",
        "string",
        ["number", "number", "number", "number", "number", "number", "number", "number", "number", "string", "number", "number", "number", "string", "string"],
        [self.data[0], self.data[1], self.data[2], self.data[3], self.data[4], self.data[5], self.data[6], self.data[7], self.data[8], self.data[9], self.data[10], self.data[11], self.data[12], self.data[13], self.data[14]]
    );
    return calc;
};
