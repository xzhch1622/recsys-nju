<?php
	include_once '../recommendersystem/keyword-recommender.php';
	include_once '../recommendersystem/keyword-recommender-system.php';
	/**
	 * wordAssociationWithJaccardPreprocess() and preprocess() involve a large number of calculations
	 * so you only need to use it in the first run
	 */ 
	 
	$keywords = "discount designer eyeglass frames";
	$test_recommend = new KeywordRecommender();
	//$test_recommend->preprocess("");
	$test_recommend->recommend($keywords);
	
	$test_recommender_sys = new KeywordRecommenderSystem();
	//$test_recommender_sys->wordAssociationWithJaccardPreprocess(0.2);// 0.2 is the jaccard factor
	$test_recommender_sys->recommend(KEY_LINK_JACCARD, $keywords);