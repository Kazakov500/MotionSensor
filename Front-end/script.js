google.charts.load('current', {'packages':['corechart']});
google.charts.setOnLoadCallback(drawChart);



var ReciveData = true;
var timeDelayForReciveData = 300; //1 sec




var dataArr = 
[
   ['Time', 'Data'],
   ['0', 0],
   ['1', 0]
];

function drawChart() 
{

   var data = google.visualization.arrayToDataTable(dataArr);

    var options = 
    {
      title: 'Напряжение на выводе',
      curveType: 'function',
      legend: 'none',
      legend: { position: 'bottom' },
      vAxis: {minValue: 0, maxValue:600000},
      hAxis: 
      {
        slantedText: true,
        slantedTextAngle: 90 /* Define slant Angle */
      },
      'chartArea': 
      { 
        'width': '82%', 
        height: '150%', 
        top: '9%', 
        left: '15%', 
        right: '3%', 
        bottom: '80'
      } /* Adjust chart alignment to fit vertical labels for horizontal axis*/
    };

    var chart = new google.visualization.AreaChart(document.getElementById('curve_chart'));

    chart.draw(data, options);
}


var tick_getDataForGCharts;
//=========== JQery =============================


$(function()
{

  
  function LoadIMG() 
  {
    var img = $("<img />").attr('src', 'http://192.168.43.135/test.svg')
    .on('load', function() {
            $("#something2").html(img);
    });
    setTimeout(LoadIMG, 1000);
  }

  setTimeout(LoadIMG, 1000);
  $("#something").on('load', function() {
      if (!this.complete || typeof this.naturalWidth == "undefined" || this.naturalWidth == 0) {
          $("#connect").html("NOT connected");
      } else {
          $("#connect").html("connected");
      }
  });
  /*
  var timerId = setTimeout(function tick() {
    var $img = $("#something");
      $img.attr("src", $img.attr("src").split("?")[0] + "?" + Math.random());
    timerId = setTimeout(tick, 3000);
  }, 3000);
  */


  function getDataForGCharts() 
  {
    var Value;
    $.get('http://192.168.43.135/getValue.html', function (data) {
      var dt = new Date();
      var time = dt.getHours() + ":" + dt.getMinutes() + ":" + dt.getSeconds();
       Value = parseInt(data)
       $("#dataVals").prepend(time + " - " + Value + "<br/>");

       dataArr.push([time, Value])
       if (dataArr.length > 50 )
          dataArr.splice(1, 1);
       drawChart();
    });
    if (ReciveData)
      setTimeout(getDataForGCharts, timeDelayForReciveData);
  }

  setTimeout(getDataForGCharts, timeDelayForReciveData);

  

  //========== BUTTONS =======================
  $( "#StartReciveData" ).click(function() 
  {
    ReciveData = true;
    setTimeout(getDataForGCharts, timeDelayForReciveData);
  });
  $( "#StopReciveData" ).click(function() 
  {
    ReciveData = false;
  });

});