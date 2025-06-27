-- MySQL dump 10.13  Distrib 8.0.42, for Win64 (x86_64)
--
-- Host: 127.0.0.1    Database: test
-- ------------------------------------------------------
-- Server version	8.0.41

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `agaab`
--

DROP TABLE IF EXISTS `agaab`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `agaab` (
  `id` int NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `agaab`
--

LOCK TABLES `agaab` WRITE;
/*!40000 ALTER TABLE `agaab` DISABLE KEYS */;
INSERT INTO `agaab` VALUES (10047),(10049),(10048),(10061),(10062),(10063),(10064),(10065),(10066),(10069);
/*!40000 ALTER TABLE `agaab` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `friendships`
--

DROP TABLE IF EXISTS `friendships`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `friendships` (
  `userid` int NOT NULL,
  `friendid` int NOT NULL,
  PRIMARY KEY (`userid`,`friendid`),
  KEY `friendid` (`friendid`),
  CONSTRAINT `friendships_ibfk_1` FOREIGN KEY (`userid`) REFERENCES `users` (`id`),
  CONSTRAINT `friendships_ibfk_2` FOREIGN KEY (`friendid`) REFERENCES `users` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `friendships`
--

LOCK TABLES `friendships` WRITE;
/*!40000 ALTER TABLE `friendships` DISABLE KEYS */;
INSERT INTO `friendships` VALUES (10022,10018),(10023,10018),(10045,10018),(10046,10018),(10018,10022),(10018,10023),(10018,10045),(10018,10046),(10048,10047),(10049,10047),(10047,10048),(10049,10048),(10061,10048),(10062,10048),(10063,10048),(10064,10048),(10065,10048),(10066,10048),(10068,10048),(10069,10048),(10047,10049),(10048,10049),(10047,10060),(10048,10060),(10049,10060),(10061,10060),(10062,10060),(10063,10060),(10064,10060),(10065,10060),(10066,10060),(10069,10060),(10048,10061),(10048,10062),(10048,10063),(10048,10064),(10048,10065),(10048,10066),(10048,10069);
/*!40000 ALTER TABLE `friendships` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `groupchat`
--

DROP TABLE IF EXISTS `groupchat`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `groupchat` (
  `id` int NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `groupchat`
--

LOCK TABLES `groupchat` WRITE;
/*!40000 ALTER TABLE `groupchat` DISABLE KEYS */;
INSERT INTO `groupchat` VALUES (10060);
/*!40000 ALTER TABLE `groupchat` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `heardmusic`
--

DROP TABLE IF EXISTS `heardmusic`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `heardmusic` (
  `num` int NOT NULL AUTO_INCREMENT,
  `id` varchar(20) DEFAULT NULL,
  `name` varchar(50) DEFAULT NULL,
  `author` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`num`)
) ENGINE=InnoDB AUTO_INCREMENT=60 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `heardmusic`
--

LOCK TABLES `heardmusic` WRITE;
/*!40000 ALTER TABLE `heardmusic` DISABLE KEYS */;
INSERT INTO `heardmusic` VALUES (1,'1830448879','带我去很远地方','黄霄雲'),(2,'1866185395','小城夏天','LBI利比'),(3,'534542079','侧脸','于果'),(4,'445154140','最后一页（爱只能在回忆里完整）','洛尘鞅_'),(5,'2648126628','年轮','不是花火呀'),(6,'2627765006','唯一','CHOCKEY有三/TizzyT'),(7,'1934251776','年轮','不是花火呀'),(8,'2151100117','小城夏天','LBI利比'),(9,'28282919','夜空中最亮的星','张恒远'),(10,'2128842362','夜空中最亮的星','Alices'),(11,'2629921305','爱错','en'),(12,'2654687535','如果爱忘了','Sky光遇凯隐'),(13,'2129904425','如果爱忘了（Live）汪苏泷/单依纯版','昕子Xinxi/浩展HAOZHAN'),(14,'2129307586','如果爱忘了','Chill5/Easy7'),(15,'2614899986','我想念1.1（xy remix）','xy/DJ阿轩/奇衡三'),(16,'2107092621','谎','Six Teen/XZ79'),(17,'1960928972','把回忆拼好给你','把回忆拼好给你'),(18,'2653125161','无名的人','张韶涵'),(19,'2137127742','无名的人','王巨星'),(20,'1943114935','无名的人','DJ阿透'),(21,'2619709335','无名的人 (Live)','毛不易'),(22,'2004985795','光落向的地方','陈恒媛'),(23,'2666095018','日落','孙燕姿'),(24,'2033326659','时光背面的我（DJ热播版）','歌者达达'),(25,'548885986','落空','印子月'),(26,'1811459887','千千万万','深海鱼子酱'),(27,'31365604','你从未离去','白挺'),(28,'1331819951','像鱼','王贰浪'),(29,'465920905','全部都是你','DP龙猪/宝楽CNBALLER/王云 Cloud Wang'),(30,'1399112638','心似烟火','陈壹千'),(31,'1381755293','山楂树之恋','程佳佳'),(32,'1883959481','山楂树之恋 (DJ版)','惠聪哲'),(33,'413829859','童话镇','暗杠'),(34,'1383927243','这一生关于你的风景','隔壁老樊'),(35,'2042256453','他不懂','王贰浪/俞天时'),(36,'421423806','小半','陈粒'),(37,'2645495145','消散对白','杜宣达'),(38,'1406649619','有些','颜人中'),(39,'1895330088','予你','队长'),(40,'436346833','凄美地','郭顶'),(41,'475479888','在你的身边','盛哲'),(42,'1368371706','你曾是少年','焦迈奇'),(43,'1363948882','世间美好与你环环相扣','柏松'),(44,'1377574592','人间城','王贰浪'),(45,'1367452194','我的一个道姑朋友','双笙（陈元汐）'),(46,'2144209098','如果可以','杜宣达'),(47,'569213220','像我这样的人','毛不易'),(48,'569212215','像我这样的人 (伴奏)','毛不易'),(49,'1397345903','悬溺','葛东琪'),(50,'2055864684','悬溺（1.1x）','萧潇'),(51,'1331535573','岩石里的花','G.E.M.邓紫棋'),(52,'432698302','多远都要在一起 (Dub Mix)','G.E.M.邓紫棋'),(53,'1392908905','追光者','岑宁儿'),(54,'1297742771','追光者','汪苏泷'),(55,'5235487','雨爱','杨丞琳'),(56,'1467444619','雨爱','不是花火呀'),(57,'543987400','小半 (Live)','周锐/郑艺彬/ONER灵超/钱正昊'),(58,'2129799489','年轮(DJ版)','我真的好想你'),(59,'1954908763','年轮','旺仔小乔');
/*!40000 ALTER TABLE `heardmusic` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `lovemusic`
--

DROP TABLE IF EXISTS `lovemusic`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `lovemusic` (
  `num` int NOT NULL AUTO_INCREMENT,
  `id` varchar(20) DEFAULT NULL,
  `name` varchar(50) DEFAULT NULL,
  `author` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`num`)
) ENGINE=InnoDB AUTO_INCREMENT=55 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `lovemusic`
--

LOCK TABLES `lovemusic` WRITE;
/*!40000 ALTER TABLE `lovemusic` DISABLE KEYS */;
INSERT INTO `lovemusic` VALUES (1,'1830448879','年轮','不是花火呀'),(2,'534542079','侧脸','于果'),(4,'445154140','最后一页（爱只能在回忆里完整）','洛尘鞅_'),(5,'1934251776','小城夏天','LBI利比'),(15,'2653125161','无名的人','张韶涵'),(17,'1866185395','带我去很远地方','黄霄雲'),(19,'2666095018','日落','孙燕姿'),(21,'2151100117','落日与晚风','辉仔Morris'),(22,'2648126628','时光背面的我','沐颜Fairy'),(24,'1381755293','山楂树之恋','程佳佳'),(27,'1399112638','心似烟火','陈壹千'),(28,'1367452194','我的一个道姑朋友','双笙（陈元汐）'),(29,'569213220','像我这样的人','毛不易'),(30,'1377574592','人间城','王贰浪'),(31,'1397345903','悬溺','葛东琪'),(32,'1331535573','岩石里的花','G.E.M.邓紫棋'),(33,'432698302','多远都要在一起 (Dub Mix)','G.E.M.邓紫棋'),(34,'1392908905','追光者','岑宁儿'),(35,'1467444619','雨爱','不是花火呀'),(54,'421423806','小半','陈粒');
/*!40000 ALTER TABLE `lovemusic` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `users`
--

DROP TABLE IF EXISTS `users`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `users` (
  `id` int NOT NULL AUTO_INCREMENT,
  `username` varchar(50) NOT NULL,
  `password` varchar(100) NOT NULL,
  `path` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=10070 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `users`
--

LOCK TABLES `users` WRITE;
/*!40000 ALTER TABLE `users` DISABLE KEYS */;
INSERT INTO `users` VALUES (10000,'111','111',NULL),(10012,'123','123',NULL),(10013,'123','12345',NULL),(10014,'1234','1231',NULL),(10015,'123','111',NULL),(10016,'1235954','1234567890',NULL),(10018,'小八嘎','234','usericon/10018.png'),(10019,'1234','1232',NULL),(10020,'124634','23243',NULL),(10021,'1123231','324235',NULL),(10022,'十八岁小帅','20041203Li?','usericon/10022.png'),(10023,'黎','123456','usericon/10023.png'),(10024,'','',NULL),(10025,'破','123456',NULL),(10026,'li','1234',NULL),(10027,'li','1234',NULL),(10028,'123456','123456',NULL),(10029,'123456','123456',NULL),(10030,'123','123',NULL),(10031,'123','123',NULL),(10032,'123','123',NULL),(10033,'123','123',NULL),(10034,'123','123',NULL),(10035,'123','123',NULL),(10036,'123','123',NULL),(10037,'123','123',NULL),(10038,'123','123',NULL),(10039,'123','123',NULL),(10040,'123','123',NULL),(10041,'123','123',NULL),(10042,'123','123',NULL),(10043,'123','123',NULL),(10044,'123','123',NULL),(10045,'1234','123','usericon/10045.png'),(10046,'哈哈','123456','usericon/10046.png'),(10047,'十八岁小美','123456','usericon/10047.png'),(10048,'十八岁小帅','123456','usericon/10048.png'),(10049,'呆妹儿','123456','usericon/10049.png'),(10060,'12345','1111',NULL),(10061,'里哈哈','123456',NULL),(10062,'哇哈哈','123456',NULL),(10063,'啦啦啦','123456',NULL),(10064,'哈哈哈','123456',NULL),(10065,'哈哈哈','123456',NULL),(10066,'哈哈哈','123456',NULL),(10067,'lalala','123456',NULL),(10068,'lalala','123456','usericon/10068.png'),(10069,'俩哈哈','123456','usericon/10069.png');
/*!40000 ALTER TABLE `users` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2025-06-27 12:02:45
