$(document).ready(function () {
    setTimeout(function () {
        $("#btnCalculate").attr("disabled", true);
    }, 0);
    var astrologerPreload = new Worker("js/swephPreload.js");
    astrologerPreload.onmessage = function (response) {
        astrologerPreload.terminate();
        $("#btnCalculate").attr("disabled", false);
    };
});
