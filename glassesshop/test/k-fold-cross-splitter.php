<?php
	include_once "../database/glass-database-manager.php";

	class KFoldCrossSplitter{
		private $dm;
		private $k;
		private $iteration_count;
		private $folds;

		public function __construct($config){
			$this->dm = GlassDatabaseManager::getInstance();
			$this->iteration_count = 0;
			$this->k = $config['k_fold'];
			if(!isset($this->k)){
				$this->k = 10;

				echo 'Dude, give me [k_fold] param<br/>';
				flush();
				ob_flush();
			}
		}

		public function start_split(){
			echo "KFoldCrossSplitter start_split start.....<br/>";
			flush();
			ob_flush();
			$time_start = microtime(true);

			$query_result = $this->dm->query("SELECT * FROM query");
			$all_query = array();
			while($all_query[] = mysql_fetch_assoc($query_result));
			array_pop($all_query); // pop the last 'false' result
			assert('count($all_query) == mysql_num_rows($query_result)');

			$shuffle_reuslt = shuffle($all_query);
			assert('$shuffle_reuslt == true');

			$this->folds = array_chunk($all_query, ceil(count($all_query) / $this->k));
			assert('count($this->folds) == $this->k');

			$time_end = microtime(true);
			$cost_time = $time_end - $time_start;
			echo 'KFoldCrossSplitter start_split end......<br/>';
			echo "KFoldCrossSplitter cost time: $cost_time <br/>";
			flush();
			ob_flush();
		}

		public function end_split(){
			echo 'KFoldCrossSplitter end_split<br/>';
			flush();
			ob_flush();
		}

		public function split(){
            $this->dm->query("BEGIN;");
			$this->iteration_count++;
			assert('$this->iteration_count <= $this->k');

			$this->dm->query("BEGIN");
			//prepare database tables
			$this->dm->query("drop table if exists query_train");
			$this->dm->query("create table query_train like query");
			$this->dm->query("drop table if exists query_test");
			$this->dm->query("create table query_test like query");

			// now $this->iteration_count - 1 is test set, others is train set
			for($i = 0; $i < $this->k; $i++){
				if($i == $this->iteration_count - 1){
					$table = 'query_test';
				}
				else{
					$table = 'query_train';
				}
				foreach($this->folds[$i] as $query){
					$this->dm->query("INSERT INTO {$table}(id, userId, query) VALUES 
									({$query['id']}, '{$query['userId']}', '{$query['query']}')");
				}
			}
			$this->dm->query("COMMIT");

			if($this->iteration_count == $this->k){
				return false; // no more iteration
			}
			else{
				return true; // still has iterations
			}
            $this->dm->query("COMMIT;");
		}
	}
?>
