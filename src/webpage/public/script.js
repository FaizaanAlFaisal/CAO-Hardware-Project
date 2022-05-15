$(function(){

    const dbRef = firebase.database();
    const messageRef = dbRef.ref('displayData');
    const templateRef = dbRef.ref('templates');

    var count = 0;

    templateRef.child('/entry' + count).on("child_added", function(snap){
        
        $('.templatesTable tbody').append(appendToTable(snap.val));
    });

    function appendToTable(info) {
        var tableRow = '<tr id="' + count + '"><td id="msg"> ' + info.message + '</td>';
        tableRow += '<td id="spd"> ' + info.scrollSpeed + '</td>';
        tableRow += '<td id="ints"> ' + info.intensity + '</td>';
        tableRow += '<td><a id="' + count + '" href="javascript:writeToDisplay();">Set Message</a></td></tr>';
    }

    function writeToDisplay() {
        console.log('test of function')
    }

    $('#submitBtn').on("click", function (event) {
        event.preventDefault(); //prevent form from submitting, stay on same page

        messageRef.remove();
        messageRef.set({ //set() sends json data to firebase rt-db for reading by esp32
            message: $('#message').val(),
            scrollSpeed: parseInt($('#scrollSpeed').val()),
            intensity: parseInt($('#intensity').val()),
            isBeingDisplayed: false 
        });
        document.getElementById('messageForm').reset(); //clear all information on form
    });

    $('#templateBtn').on("click", function (event) {
        event.preventDefault(); //prevent form from submitting, stays on same page

        templateRef.child('/entry' + count++).set({
            message: $('#message').val(),
            scrollSpeed: parseInt($('#scrollSpeed').val()),
            intensity: parseInt($('#intensity').val())    
        });
        
        document.getElementById('messageForm').reset(); //clear all information on form
    });

});