<?php

	if($sy) {
	
		$psy = $sy - 1; //previous sy
	


		if($glvl) {

			$sql = "
				SELECT 
				stuid, 
				enrdate,
				SUM(IF(sy=${sy}, schid, 0)) AS schidThisYear,
				SUM(IF(sy=${psy}, glvl, 0)) AS glvlLastYear,
				SUM(IF(sy=${sy}, glvl, 0)) AS glvlThisYear,
				SUM(IF(sy=${sy} AND enrtype='x', 1, 0)) AS droppedThisYear 
				FROM people.stuenroll_static 
				WHERE sy IN (${psy}, ${sy}) AND enrtype!='ns' 
				GROUP BY stuid 
				HAVING glvlThisYear = ${glvl}
				";

			if($schid) $sql .= " AND schidThisYear=${schid}";

			if($_GET["a"]) printf($sqlPrintStr, "detail sql:<br />\n" . str_replace("<", "&lt;", $sql));

			$data["glvl"][0] = array("sy"=>$sy, "schid"=>$schid, "glvl"=>$glvl, "N"=>0);
			$data["glvl"]["rpt"] = array();
			$data["glvl"]["drop"] = array();
			$data["glvl"]["new"] = array();

			$rs = $db->query($sql) or die("by grade detail: " . $db->error);
			
			$data["glvl"][0]["N"] = $rs->num_rows;
			
			if($_GET["a"]) printf($sqlPrintStr, "Number of Records Found: " . $rs->num_rows);
			
			while($tmp = $rs->fetch_assoc()) {
				$lGLvl = $tmp["glvlLastYear"];
				$tGLvl = $tmp["glvlThisYear"];
				if($lGLvl && $tGLvl <= $lGLvl) {
					$data["glvl"]["rpt"][] = array(
												"stuid"=>$tmp["stuid"], 
												"stuname"=>$tmp["stuname"],
												"glvl"=>$tGLvl,
												"date"=>$tmp["enrdate"]
												);
				} 
				if($tmp["droppedThisYear"]) {
					$data["glvl"]["drop"][] = array(
												"stuid"=>$tmp["stuid"], 
												"stuname"=>$tmp["stuname"],
												"glvl"=>$tmp["glvlThisYear"],
												"date"=>$tmp["enrdate"]
												);
				} 
				if(!$lGLvl && $tGLvl > 1) {
					$data["glvl"]["new"][] = array(
												"stuid"=>$tmp["stuid"], 
												"stuname"=>$tmp["stuname"],
												"glvl"=>$tmp["glvlLastYear"] . ":" . $tmp["glvlThisYear"],
												"date"=>$tmp["enrdate"]
												);
				} 
			}

		} else {
			
			$sql = "
				SELECT
					glvlThisYear, COUNT(*) AS N, 
					SUM(IF(glvlLastYear AND glvlThisYear > glvlLastYear, 1, 0)) AS promoted,
					SUM(IF(glvlLastYear AND glvlThisYear <= glvlLastYear, 1, 0)) AS retained,
					SUM(droppedThisYear) AS dropped,
					SUM(IF(glvlThisYear > 1 AND glvlLastYear = 0, 1, 0)) AS newIn
				FROM
					(
						SELECT 
						stuid, 
						SUM(IF(sy=${sy}, schid, 0)) AS schidThisYear,
						SUM(IF(sy=${psy}, glvl, 0)) AS glvlLastYear,
						SUM(IF(sy=${sy}, glvl, 0)) AS glvlThisYear,
						SUM(IF(sy=${sy} AND enrtype='x', 1, 0)) AS droppedThisYear 
						FROM people.stuenroll_static 
						WHERE sy IN (${psy}, ${sy}) AND enrtype!='ns' 
						GROUP BY stuid 
					) a
				WHERE glvlThisYear > 0 
				GROUP BY glvlThisYear		
			";
			
			if($schid) $sql = str_replace("WHERE glvl", "WHERE schidThisYear=${schid} AND glvl", $sql);

			if($_GET["a"]) printf($sqlPrintStr, str_replace("<", "&lt;", $sql));

			$rs = $db->query($sql) or die($db->error);

			for($g=1; $g<=8; $g++) {
				$data[$g] = array("glvl"=>$g, "N"=>0, "rpt"=>0, "drop"=>0, "new"=>0, "rpt%"=>0, "drop%"=>0, "new%"=>0);
			}

			while($tmp = $rs->fetch_assoc()) {
				$data[$tmp["glvlThisYear"]]["glvl"] = $tmp["glvlThisYear"];
				$data[$tmp["glvlThisYear"]]["N"] = $tmp["N"];
				$data[$tmp["glvlThisYear"]]["rpt"] = $tmp["retained"];
				$data[$tmp["glvlThisYear"]]["drop"] = $tmp["dropped"];
				$data[$tmp["glvlThisYear"]]["new"] = $tmp["newIn"];
				if($tmp["N"]) {
					$data[$tmp["glvlThisYear"]]["rpt%"] = round($tmp["retained"] / $tmp["N"], 3);
					$data[$tmp["glvlThisYear"]]["drop%"] = round($tmp["dropped"] / $tmp["N"], 3);
					$data[$tmp["glvlThisYear"]]["new%"] = round($tmp["newIn"] / $tmp["N"], 3);
				}
			}
		}
	}
?>