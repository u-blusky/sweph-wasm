self.Module = {
    onRuntimeInitialized: function () {
        postMessage("finish");
    },
};
self.importScripts("astro.js");
