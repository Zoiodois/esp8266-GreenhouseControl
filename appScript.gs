//This code MUST be in your spreasheet and deployed as an SpreadsheetApp


function doGet(e) {
  Logger.log(e);
  var result = 'Ok';
  var parameter = e.parameter

  if (typeof parameter === 'string' && parameter.includes("temperature")) {
    postData(parameter)
  } else {
    returnData();
  }


  /*
   else {
     var sheet_id = '17-Eno49CQ6jnEC5h1iZSUX54uJkpU3MjQmqVx3Ppxw4'; // Spreadsheet ID
     var sheet = SpreadsheetApp.openById(sheet_id).getSheetByName("Recebedor");
     var newRow = sheet.getLastRow() + 1;
     var rowData = [];
     var Curr_Date = new Date();
     rowData[0] = Curr_Date; // Date in column A
     var Curr_Time = Utilities.formatDate(Curr_Date, "America/Sao_Paulo", 'HH:mm:ss');
     rowData[1] = Curr_Time; // Time in column B
     for (var param in e.parameter) {
       //Logger.log('In for loop, param=' + param);
       var value = stripQuotes(e.parameter[param]);
       //Logger.log(param + ':' + e.parameter[param]);
       switch (param) {
         case 'temperature':
           rowData[2] = value; // Temperature in column C
           result = 'Temperature Written on column C';
           break;
         case 'humidity':
           rowData[3] = value; // Humidity in column D
           result += ' ,Humidity Written on column D';
           break;
         case 'temperaturaTerm':
           rowData[4] = value; // Temperatura Varistor in column G
           result += ' ,Temperatura Varistor Written on column G';
           break;
         case 'UmidadeSolo':
           rowData[5] = value; // Humidity in column F
           result += ' ,Umidade do Solo Written on column F';
           break;
         case 'Luminosidade':
           rowData[6] = value; // Luminosidade in column H
           result += ' ,Luminosidade Written on column H';
           break;
         case 'UmidadeSolo2':
           rowData[7] = value; // Luminosidade in column I
           result += ' ,Umidade do Solo Written on column I';
           break;
 
         default:
           result = "unsupported parameter";
       }
     }
     //Logger.log(JSON.stringify(rowData));
     var newRange = sheet.getRange(newRow, 1, 1, rowData.length);
     newRange.setValues([rowData]);
   }
   return ContentService.createTextOutput(result);
   */
}

function postData(dados) {

  var sheet_id = '17-Eno49CQ6jnEC5h1iZSUX54uJkpU3MjQmqVx3Ppxw4'; // Spreadsheet ID
  var sheet = SpreadsheetApp.openById(sheet_id).getSheetByName("Recebedor");
  var newRow = sheet.getLastRow() + 1;
  var rowData = [];
  var Curr_Date = new Date();
  rowData[0] = Curr_Date; // Date in column A
  var Curr_Time = Utilities.formatDate(Curr_Date, "America/Sao_Paulo", 'HH:mm:ss');
  rowData[1] = Curr_Time; // Time in column B
  for (var param in dados) {
    //Logger.log('In for loop, param=' + param);
    var value = stripQuotes(dados[param]);
    //Logger.log(param + ':' + dados[param]);
    switch (param) {
      case 'temperature':
        rowData[2] = value; // Temperature in column C
        result = 'Temperature Written on column C';
        break;
      case 'humidity':
        rowData[3] = value; // Humidity in column D
        result += ' ,Humidity Written on column D';
        break;
      case 'temperaturaTerm':
        rowData[4] = value; // Temperatura Varistor in column G
        result += ' ,Temperatura Varistor Written on column G';
        break;
      case 'UmidadeSolo':
        rowData[5] = value; // Humidity in column F
        result += ' ,Umidade do Solo Written on column F';
        break;
      case 'Luminosidade':
        rowData[6] = value; // Luminosidade in column H
        result += ' ,Luminosidade Written on column H';
        break;
      case 'UmidadeSolo2':
        rowData[7] = value; // Luminosidade in column I
        result += ' ,Umidade do Solo Written on column I';
        break;

      default:
        result = "unsupported parameter";
    }
  }
  //Logger.log(JSON.stringify(rowData));
  var newRange = sheet.getRange(newRow, 1, 1, rowData.length);
  newRange.setValues([rowData]);
  return ContentService.createTextOutput(result);

}

function returnData(dados) {  

  Logger.log("rodou return data")
 
  var sheet_id = '17-Eno49CQ6jnEC5h1iZSUX54uJkpU3MjQmqVx3Ppxw4';// Spreadsheet ID.
  var sheet_name = "Tempos";// Sheet Name in Google Sheets.
  var sheet_open = SpreadsheetApp.openById(sheet_id);
  var sheet_target = sheet_open.getSheetByName(sheet_name);

  
    var all_Data = sheet_target.getRange('A1:B1').getValues();
    var resp = "ta indo"
    //return ContentService.createTextOutput("ta indo");
    return  ContentService.createTextOutput(resp)
    .setMimeType(ContentService.MimeType.TEXT);
}



function stripQuotes(value) {
  return value.replace(/^["']|['"]$/g, "");
}