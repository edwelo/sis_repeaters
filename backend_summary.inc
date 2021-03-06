<?php

	if($sy) {

		$data["sy"][0] = array("sy"=>$sy, "schid"=>$schid);
		$data["sy"]["rpt"] = array();
		$data["sy"]["drop"] = array();
		$data["sy"]["new"] = array();

		if($sy > 2009) {
			$psy = $sy - 1;

			//list of repeater students
			$sql = "
				SELECT 
					a.id AS stuid, CONCAT(a.fname, ' ', a.lname) AS stuname,
					b.stusex, b.schid, b.glvl, b.sec, b.enrid, b.enrtype, b.enrdate,
					c.glvl AS pGlvl
				FROM 
					people.students a,
					people.stuenroll_static b,
					(SELECT t1.* FROM people.stuenroll_static t1 
						WHERE t1.sy=${psy} AND t1.glvl IS NOT NULL 
						AND t1.glvl>0 AND t1.enrtype!='ns') c
				WHERE 
					a.id=b.stuid AND b.stuid=c.stuid AND 
					b.sy=${sy} AND b.glvl>0 AND 
					b.enrtype!='ns' AND b.glvl=c.glvl	
				ORDER BY b.glvl, stuname	
			";
			if($schid) {
				$search = "b.sy=${sy} AND";
				$replace = "b.sy=${sy} AND b.schid=${schid} AND ";
				$sql = str_replace($search, $replace, $sql);
			}

			if($_GET["a"]) printf($sqlPrintStr, "repeaters:<br />\n" . str_replace("<", "&lt;", $sql));
	
			$rs = $db->query($sql) or die("backend repeaters: " . $db->error);
			while($tmp = $rs->fetch_assoc()) {
				$data["sy"]["rpt"][] = 
				array(
					"stuid"=>$tmp["stuid"],
					"stuname"=>$tmp["stuname"],
					"glvl"=>$tmp["glvl"],
					"date"=>$tmp["enrdate"]
				);
			}

			//list of new students
			$sql = "
				SELECT 
					a.id AS stuid, CONCAT(a.fname, ' ', a.lname) AS stuname, 
					b.stusex, b.schid, b.glvl, b.sec, b.enrid, 
					b.enrtype, b.enrdate, c.glvl AS pglvl
				FROM 
					people.students a, people.stuenroll_static b LEFT JOIN 
					(SELECT t1.* FROM people.stuenroll_static t1 
						WHERE t1.sy=${psy} AND t1.glvl IS NOT NULL 
							AND t1.glvl>0 AND t1.enrtype!='ns') c ON b.stuid=c.stuid 
				WHERE 
					a.id=b.stuid AND b.sy=${sy} AND b.glvl>1
					AND b.glvl IS NOT NULL AND b.enrtype!='ns' AND c.stuid IS NULL
				ORDER BY b.glvl, stuname	
			";
			if($schid) {
				$search = "b.sy=${sy} AND";
				$replace = "b.sy=${sy} AND b.schid=${schid} AND ";
				$sql = str_replace($search, $replace, $sql);
			}

			if($_GET["a"]) printf($sqlPrintStr, "new:<br />\n" . str_replace("<", "&lt;", $sql));
	
			$rs = $db->query($sql) or die("backend repeaters: " . $db->error);
			while($tmp = $rs->fetch_assoc()) {
				$data["sy"]["new"][] = 
				array(
					"stuid"=>$tmp["stuid"],
					"stuname"=>$tmp["stuname"],
					"glvl"=>$tmp["glvl"],
					"date"=>$tmp["enrdate"]
				);
			}
		}

		//list of drop students
		$sql = "SELECT CONCAT(a.fname, ' ', a.lname) AS stuname, b.* 
				FROM people.students a, people.stuenroll_static b 
				WHERE a.id=b.stuid AND b.sy=${sy} AND b.enrtype!='ns' 
				ORDER BY b.glvl, stuname";
		if($schid) {
			$search = "b.sy=${sy} AND";
			$replace = "b.sy=${sy} AND b.schid=${schid} AND ";
			$sql = str_replace($search, $replace, $sql);
		}
	
		if($_GET["a"]) printf($sqlPrintStr, "drops:<br />\n" . str_replace("<", "&lt;", $sql));
	
		$rs = $db->query($sql) or die("backend dropped: " . $db->error);
		while($tmp = $rs->fetch_assoc()) {
			if($tmp["enrtype"] == "x" || $tmp["enrtype"] == "ns") {
				$data["sy"]["drop"][] = 
				array(
					"stuid"=>$tmp["stuid"],
					"stuname"=>$tmp["stuname"],
					"glvl"=>$tmp["glvl"],
					"date"=>$tmp["enrdate"]
				);
			}
		}

	} else {

		$sql = "SELECT MIN(sy) AS minYr, MAX(sy) AS maxYr FROM people.stuenroll_static";
		$rs = $db->query($sql) or die("backend get min max yrs: " . $db->error);
		$tmp = $rs->fetch_assoc();
		$minYear = $tmp["minYr"]; $maxYear = $tmp["maxYr"];

		for($y=$minYear; $y<=$maxYear; $y++) {
			$tmp1 .= ", SUM(IF(sy=" . $y . ", schid, 0)) AS '" . $y . "s'";
			$tmp2 .= ", SUM(IF(sy=" . $y . ", glvl, 0)) AS '" . $y . "g'";
			$tmp3 .= ", SUM(IF(sy=" . $y . " AND enrtype='x', 1, 0)) AS '" . $y . "d'";
		}
		$sql = "SELECT stuid " . $tmp1 . $tmp2 . $tmp3 . " FROM people.stuenroll_static WHERE enrtype!='ns' GROUP BY stuid";

		/*
		if($schid) {
			$search = "GROUP BY";
			$replace = "AND schid=${schid} GROUP BY";
			$sql = str_replace($search, $replace, $sql);
		}
		*/

		if($_GET["a"]) printf($sqlPrintStr, str_replace("<", "&lt;", $sql));

		$rs = $db->query($sql) or die($db->error);

		for($y=$minYear; $y<=$maxYear; $y++) {
			$data[$y] = array("sy"=>$y, "N"=>0, "rpt"=>0, "drop"=>0, "new"=>0, "rpt%"=>0, "drop%"=>0, "new%"=>0);
		}

		while($tmp = $rs->fetch_assoc()) {
			for($y=$minYear; $y<=$maxYear; $y++) {
				if($schid) { if($schid != $tmp[$y . "s"]) continue; }
				$py = $y - 1;
				if($tmp[$y . "g"]) {
					$data[$y]["N"]++;
					if(isset($tmp[$py . "g"]) && $tmp[$y . "g"] == $tmp[$py . "g"]) $data[$y]["rpt"]++;
					if($tmp[$y . "d"]) $data[$y]["drop"]++;
					if(isset($tmp[$py . "g"]) && $tmp[$y . "g"] > 1 && !$tmp[$py . "g"]) $data[$y]["new"]++;			
				}
			}
		}

		//calculate the percentages
		foreach($data as $sy=>$row) {
			$data[$sy]["rpt%"] = round($row["rpt"] / $row["N"], 3);
			$data[$sy]["drop%"] = round($row["drop"] / $row["N"], 3);
			$data[$sy]["new%"] = round($row["new"] / $row["N"], 3);
		}
	}
?>