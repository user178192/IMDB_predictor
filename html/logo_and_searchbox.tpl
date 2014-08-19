    <style type="text/css">
        
        #btnSubmit
        {
            height: 37px;
            width: 94px;
        }
        #txtquery
        {
            height:37px;
            width:60%;
        }
        #selectType
        {
            height:37px;
            vertical-align:middle;
            font-size:large; 
            width:100px;
        }
        #logo
        {
            text-align:center; 
            font-size:xx-large; 
            font-style:italic;
            font-family:Arial,Helvetica,sans-serif;
        }                       
    </style>

    <div id="showLogo">
    <p id="logo" >7Movie.me</p>
    </div>
    <form action="/query" method="get" enctype="text/plain" id="form1">
        <div id="searchDiv">
            <table style="width:80%; height:30px" align="left">
                <tr>
                    <td style="width:60%" align="right">
                        <input id="txtquery" style="font-size:large" type="text" name="keywords">
                    </td>
                    <td style="width:3%">
                        <select id="selectType" name="query_type">
                            <option value="text" style="font-size:large">Movie</option>
                            <option value="people" style="font-size:large">People</option>
                        </select>
                    </td>
                    <td style="width:10%" align="left">
                        <input id="btnSubmit" type="submit" size="100%" value="Search" >
                    </td>
                </tr>                
            </table>
        </div>
        <br />
    </form>

