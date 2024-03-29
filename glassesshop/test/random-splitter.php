<?php
	include_once "../database/glass-database-manager.php";

	class RandomSplitter{
		private $dm;
		private $trainPercentage;
		private $testPercentage;

		public function __construct($config){
			$this->dm = GlassDatabaseManager::getInstance();
			$this->trainPercentage = $config['train_percentage'];
			$this->testPercentage = $config['test_percentage'];

			if(!isset($this->trainPercentage)){
				$this->trainPercentage = 0.7;
				
				echo 'Dude, give me [train_percentage] param<br/>';
				flush();
				ob_flush();
			}

			if(!isset($this->testPercentage)){
				$this->testPercentage = 0.3;

				echo 'Dude, give me [test_percentage] param<br/>';
				flush();
				ob_flush();
			}
		}

		public function start_split(){
			echo 'RandomSplitter start_split<br/>';
			flush();
			ob_flush();
		}

		public function end_split(){
			echo 'RandomSplitter end_split<br/>';
			flush();
			ob_flush();
		}

		/**
		 * split query table into query_train(train set) and query_test(test set)
		 */
		public function split(){
			$this->dm->query("BEGIN;");
			echo "RandomSplitter split start.....<br/>";
			flush();
			ob_flush();
			$time_start = microtime(true);

			$this->dm->query("BEGIN");
			assert('$this->trainPercentage + $this->testPercentage == 1');
			// prepare database tables
			$this->dm->query("drop table if exists query_train");
			$this->dm->query("create table query_train like query");
			$this->dm->query("drop table if exists query_test");
			$this->dm->query("create table query_test like query");

			// get size of test set
			$query_count_results = $this->dm->query("select count(id) query_count from query");
			$query_count_row = mysql_fetch_array($query_count_results);
			$query_count = $query_count_row['query_count'];
			$test_count = round($query_count * $this->testPercentage);

			// fill query_test and query_train
			$this->dm->query("insert into query_train select * from query");
			$test_query_results = $this->dm->query("select * from query_train order by rand() limit $test_count");

			while($test_query_row = mysql_fetch_array($test_query_results)){
				$this->dm->query("insert into query_test (id, userId, query) 
								 values ({$test_query_row['id']}, '{$test_query_row['userId']}', '{$test_query_row['query']}')");
			}
			$this->dm->query("COMMIT");

			$time_end = microtime(true);
			$cost_time = $time_end - $time_start;
			echo 'RandomSplitter split end......<br/>';
			echo "RandomSplitter cost time: $cost_time <br/>";
			flush();
			ob_flush();
			$this->dm->query("COMMIT;");

			// no more iterations
			return false;
		}
	}
?>
