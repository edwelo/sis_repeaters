	<!-- list section -->
	<div id="list" class="div_body">

		Grade Level Counts
		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
		<span style="font-size: small; color: grey;"> Year: </span>
		<select id="schYr" onchange="main();">
			<option>2018</option>
			<option>2017</option>
			<option>2016</option>
			<option>2015</option>
			<option>2014</option>
			<option>2013</option>
			<option>2012</option>
			<option>2011</option>
			<option>2011</option>
			<option>2009</option>
		</select>

		&nbsp;&nbsp;&nbsp;
		<span style="font-size: small; color: grey;"> School: </span>
		<select id="schId" onchange="main();">
			<option value=''>All Schools</option>
			<option value='11'>Aimeliik</option>
			<option value='7'>Airai</option>
			<option value='17'>Angaur</option>
			<option value='12'>GBH</option>
			<option value='10'>Ibobang</option>
			<option value='1'>JFK</option>
			<option value='13'>Koror</option>
			<option value='5'>Melekeok</option>
			<option value='15'>Meyuns</option>
			<option value='3'>Ngaraard</option>
			<option value='2'>Ngarchelong</option>
			<option value='8'>Ngardmau</option>
			<option value='6'>Ngchesar</option>
			<option value='9'>Ngeremlengui</option>
			<option value='4'>Ngiwal</option>
			<option value='16'>Peleliu</option>
			<option value='14'>PHS</option>
			<option value='19'>Puloana</option>
			<option value='18'>Sonsorol</option>
			<option value='20'>Tobi</option>			
		</select>

		<table id="listTable" class="tbl">
			<caption style="display: none;">
				glvl,N,rpt,drop,new,rpt%,drop%,new%
			</caption>
			<tr>
				<th>Grade</th>
				<th>N</th>
				<th>Rpt</th>
				<th>Drop</th>
				<th>New</th>
				<th>Rpt%</th>
				<th>Drop%</th>
				<th>New%</th>
			</tr>
			<tr style="vertical-align: top; cursor: pointer;">
				<td style="text-align: center"></td>
				<td style="text-align: center"></td>
				<td style="text-align: center"></td>
				<td style="text-align: center"></td>
				<td style="text-align: center"></td>
				<td style="text-align: center"></td>
				<td style="text-align: center"></td>
				<td style="text-align: center"></td>
			</tr>
		</table>
	</div>

	<!-- detail section -->
	<div id="detail" class="div_body" style="display: none;">
		School Year: <span id="detailSY"></span>&nbsp;&nbsp;
		Grade Level: <span id="detailGLvl"></span><br />
		<div style="float: left; width: 33%;">
			<h3>Repeat Students (<span id="rCount"></span>)</h3>
			<div id="rptList">
			
			</div>
		</div>
		<div style="float: left; width: 33%;">
			<h3>Dropped Students (<span id="dCount"></span>)</h3>
			<div id="dropList">
			
			</div>
		</div>
		<div style="float: left; width: 33%;">
			<h3>New Students (<span id="nCount"></span>)</h3>
			<div id="newList">
			
			</div>
		</div>
		<div style="clear: both;"></div>
	</div>
