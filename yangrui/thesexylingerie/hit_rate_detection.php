<?php
	require 'search.php';
	
	/** ���޸Ĳ���:����ʱȷ��ȡ�Ƽ��б��ǰ��λ Ĭ��20λ */
	$test_factor = 20;
	
	$product_identifier = "http://www.thesexylingerie.co.uk/product/";

	$con = mysql_connect($db_host , $db_user, $db_pass);
	if(!$con){
	    die(mysql_error());
	}
	mysql_select_db('thesexylingerie_test');
	
	$result = mysql_query("SELECT distinct userid,keywords FROM preprocessed_user_test");
	$all = 0;//��¼��Ч�����û���
	$hit_num = 0;//��¼���Լ���ʵ��������Ƽ��б��е���Ʒ���û�����
	
	if(!$result){
	    die('no result available');
	}
	else{
		while($row = mysql_fetch_array($result)){
			$all++;
			$userid = $row['userid'];
			$keywords = $row['keywords'];
			
			echo $userid."<br />";
			
			$product = recommendation_list($keywords);
			$hit_break = false;
			$null_break = true;
			
			$page_result = mysql_query("select page from visit where page LIKE '{$product_identifier}%' and userid = ".$userid);
			while($page_row = mysql_fetch_array($page_result)){
				//echo $userid." ".$page_row[0]."<br />";
				$i = 0;
				$null_break = false;
				foreach($product as $p_name => $p_weight){
					$hit_break = false;
					$i++;
					echo $p_name."<br />";
					if($i > $test_factor){
						break;
					}
					if($p_name == $page_row[0])
					{
						$hit_num++;
						echo "hit!<br />";
						$hit_break = true;
						break;
					}
				}
				if($hit_break)
					break;
			}
			if($null_break)
				$all--;
		}
	}
	
	echo "hit_rate = ".$hit_num."/".$all." = ".$hit_num/$all;
	
	
	