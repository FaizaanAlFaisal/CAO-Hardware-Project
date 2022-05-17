$(function(){

    const dbRef = firebase.database();
    const messageRef = dbRef.ref('displayData');
    const templateRef = dbRef.ref('templates');

    var templateCount = 0; //keeps track of number of templates that already exist

    //helps usability for new-users, easy to understand simple words instead of ranges of values
    var intensities = {"Low":2, "Medium":6, "High":10}; 
    var speeds = {"Slow":500, "Moderate":250, "Fast":150}; 
    //key-value pairs make conversion simple in coding

    templateRef.on("child_added", function(snap){
        $('.templatesTable tbody').append(appendToTable(snap.val()));
    });

    function appendToTable(info) {
        // console.log("Adding template: " + info);
        var spd = ""; var ints = "";
        if (info.scrollSpeed == 500) spd = "Slow"; else if (info.scrollSpeed == 250) spd = "Moderate"; else spd = "Fast";
        if (info.intensity == 2) ints = "Low"; else if (info.intensity == 6) ints = "Medium"; else ints = "High"; 


        var tableRow = '<tr id="' + templateCount + '"><td class="msg">' + info.message + '</td>';
        tableRow += '<td class="spd">' + spd + '</td>';
        tableRow += '<td class="ints">' + ints + '</td>';
        tableRow += '<td><button id="' + templateCount + '" class="btn btn-primary p-1 m-1 useTemplate">Set Message</button></td></tr>';

        templateCount++; //increment counter of number of templates

        return tableRow;
    }

    $('#submitBtn').on("click", function (event) {
        event.preventDefault(); //prevent form from submitting, stay on same page

        messageRef.remove();
        messageRef.set({ //set() sends json data to firebase rt-db for reading by esp32
            message: $('#message').val(),
            scrollSpeed: speeds[$('#scrollSpeed').val()],
            intensity: intensities[$('#intensity').val()],
            isBeingDisplayed: false 
        });
        document.getElementById('messageForm').reset(); //clear all information on form
    });

    $('#templateBtn').on("click", function (event) {
        event.preventDefault(); //prevent form from submitting, stays on same page

        templateRef.child('/entry' + templateCount).set({
            message: $('#message').val(),
            scrollSpeed: speeds[$('#scrollSpeed').val()],
            intensity: intensities[$('#intensity').val()]   
        });
        
        document.getElementById('messageForm').reset(); //clear all information on form
    });

    //since elements are appended after the fact, use DOM to check for them, instead of directly
    $(document).on("click", "button.useTemplate", function (event) { //when the "set message" link is clicked
        event.preventDefault(); //prevent click from redirecting page

        var rowID = event.target.id; //fetch id of anchor clicked, this id matches table row id
        var templateMessage = $('tr#' + rowID + " td.msg").text(); //using row id, get message, scroll speed, intensity
        var templateScrollSpeed = $('tr#' + rowID + " td.spd").text();
        var templateIntensity = $('tr#' + rowID + " td.ints").text();

        console.log("Template: " + templateMessage + ", " + templateScrollSpeed + ", " + templateIntensity);

        messageRef.remove(); //remove the previously displayed message form LED matrix
        messageRef.set({ //set() sends json data to firebase rt-db for reading by esp32
            message: templateMessage,
            scrollSpeed: speeds[templateScrollSpeed],
            intensity: intensities[templateIntensity],
            isBeingDisplayed: false 
        });
    });

});
