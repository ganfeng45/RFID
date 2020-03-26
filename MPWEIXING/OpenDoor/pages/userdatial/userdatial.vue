<template>
	<view class="content">
		<view v-if="showxg == 0">参数传递错误</view>
		<view class="ryxq" v-if="showxq != 0">
			<view class="xq-list">
				<view class="items">最后更新：{{user.user_regtime}}</view>
			</view>
			<view class="xq-title" style="position: relative;">人员信息</view>
			<view class="xq-list">
				<view class="items">姓名：{{user.user_name}}</view>
				<view class="items">性别：{{user.user_sex}}</view>
				<view class="items">是否本地户籍：{{user.user_statue}}</view>
				<!-- <view class="items">年龄：{{ ryinfo.age}}</view> -->
				<view class="items">证件类型：{{ ryinfo.id_type||'普通卡'}}</view>
				<view class="items">证件号码：{{ user.user_uid || '未知' }}</view>
				<view class="items">联系电话：{{user.user_phone}}</view>
			</view>
			<view class="xq-title">到访信息</view>
			<view class="xq-list">
				<view class="items">本单位联系人：{{ ryinfo.access.name || '无' }}</view>
				<view class="items">联系人电话：{{ ryinfo.access.phone || '无' }}</view>
				<view class="items">到访事由：{{ ryinfo.access.comment || '无' }}</view>
			</view>
			<view class="xq-title">来源地信息</view>
			<view class="xq-list">
				<view class="items">是否来自武汉：{{ ryinfo.from_wh}}</view>
				<view class="items">是否来自湖北：{{ ryinfo.from_hb}}</view>
				<!-- <view class="items">出行方式：{{ryinfo.traffic.type|trafficType}}</view> -->
				<view class="items">车牌或列车号：{{ ryinfo.traffic.car_plate || '未知' }}</view>
				<view class="items">来源：{{ ryinfo.from_address}}</view>
				<view class="items">现居：{{ ryinfo.check_in_address}}</view>
			</view>
			<view class="xq-title">健康状况</view>
			<view class="xq-list">
				<view class="items">体温：{{ ryinfo.temperature }}</view>
			</view>
		</view>
	</view>
</template>

<script>
export default {
	data() {
		return {
			ryinfo: {},
			showxg: 1,
			showxq: 1,
			user:[]
		};
	},
	onLoad({ item } = {}) {
		try {
			const ryinfo = Object.assign(this.ryinfo, JSON.parse(decodeURIComponent(item)));
			if (!ryinfo.list_id) {
				//this.ryinfo.user_name=ryinfo.user_name;
				// console.log('开始测试');
				// console.log(ryinfo);
				//this.user.name=ryinfo.user_name;
				// console.log('这是名字');
				// console.log(this.user.name);
				//this.loadData(ryinfo);
				this.user=ryinfo;
			}
		} catch (e) {
			console.error(e);
		}
	},
	methods: {
		loadData(id) {
			//this.ryinfo.user_name='test';
			// this.user=id;
			console.log('打印目标开始1111');
			console.log(this.ryinfo);
			
			console.log(this.user.name);
		},
	}
};
</script>

<style>
page {
	background: #ffffff;
}

.ryxq {
	display: flex;
	width: 750upx;
	/* height: 100%; */
	background: #ffffff;

	/* position: fixed;
		z-index: 99;
		margin-top: 0upx;
		top: 0upx; */
	flex-direction: column;
	overflow: auto;
}

.xq-list {
	flex-direction: row;
	font-size: 30upx;
	line-height: 60upx;
	flex-wrap: wrap;
	margin-top: 20upx;
	height: auto;
	margin-bottom: 20upx;
}

.items {
	float: left;
	overflow: hidden;
	background: #efefef;
	height: 50upx;
	line-height: 50upx;
	margin: 20upx 20upx 0upx 20upx;
	padding: 5upx 15upx 5upx 15upx;
	border-radius: 20upx;
}

.xq-title {
	padding: 20upx;
	border-bottom: #cccccc 2upx solid;
}
</style>
