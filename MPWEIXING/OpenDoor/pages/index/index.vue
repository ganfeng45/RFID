<template>
	<view class="content">
		<!-- 图表/列表切换卡 -->
		<view class="uni-padding-wrap">
			<uni-segmented-control :current="current" :values="items" :style-type="styleType" :active-color="activeColor" @clickItem="onClickItem" />
			
		</view>
		<!-- 简单列表 -->
		<!-- <view class="" v-if="!current" class="simplelist"> -->
		<you-scroll v-if="!current" ref="scroll" @onPullDown="onPullDown" @onScroll="onScroll" @onLoadMore="onLoadMore">
			<view>
				<uni-list>
					<view v-if="!members.length" class="none">暂无数据</view>
					<uni-list-item
						v-for="(item, index) in members"
						:key="index"
						@click="openDetail(item)"
						:color="listcolor"
						:title="item.user_name || '未知人员'"
						:note="item.readID || '地址未填写' + item.readID"
					></uni-list-item>
				</uni-list>
				<!-- <div class="loadingText">正在努力加载...</div> -->
			</view>
		</you-scroll>

		<!-- 图文列表 -->
		<view v-if="current" class="listview">
			图文列表
			<button type="primary" @tap="totestPAGE()">totest</button>
			<view class="qiun-columns">
				<!--#ifdef H5 -->
				<view class="qiun-bg-white qiun-title-bar qiun-common-mt"><view class="qiun-title-dot-light">页面地址</view></view>
				<view class="qiun-bg-white qiun-padding"><text>pages/basic/line/line-scroll</text></view>
				<!--#endif-->
				<view class="qiun-bg-white qiun-title-bar qiun-common-mt"><view class="qiun-title-dot-light">进/出人数（小时）</view></view>
				<view class="qiun-charts">
					<!--#ifdef MP-ALIPAY -->
					<canvas
						canvas-id="canvasLineA"
						id="canvasLineA"
						class="charts"
						:width="cWidth * pixelRatio"
						:height="cHeight * pixelRatio"
						:style="{ width: cWidth + 'px', height: cHeight + 'px' }"
						disable-scroll="true"
						@touchstart="touchLineA"
						@touchmove="moveLineA"
						@touchend="touchEndLineA"
					></canvas>
					<!--#endif-->
					<!--#ifndef MP-ALIPAY -->
					<canvas
						canvas-id="canvasLineA"
						id="canvasLineA"
						class="charts"
						disable-scroll="true"
						@touchstart="touchLineA"
						@touchmove="moveLineA"
						@touchend="touchEndLineA"
					></canvas>
					<!--#endif-->
					<view class="qiun-bg-white qiun-title-bar qiun-common-mt"><view class="qiun-title-dot-light">标准数据格式</view></view>
					<view class="qiun-bg-white qiun-padding"><textarea class="qiun-textarea" auto-height="true" maxlength="-1" v-model="textarea" /></view>
					<view class="qiun-text-tips">Tips：修改后点击更新图表</view>
					<button class="qiun-button" @tap="changeData()">更新图表</button>
				</view>
				<!--#ifdef H5 -->
				<view class="qiun-bg-white qiun-title-bar qiun-common-mt"><view class="qiun-title-dot-light">标准数据格式</view></view>
				<view class="qiun-bg-white qiun-padding"><textarea class="qiun-textarea" auto-height="true" maxlength="-1" v-model="textarea" /></view>
				<view class="qiun-text-tips">Tips：修改后点击更新图表</view>
				<button class="qiun-button" @tap="changeData()">更新图表</button>
			 	<button type="primary" class="qiun-button" @click="totestPAGE()"></button>
				<!--#endif-->
			</view>
		</view>
	</view>
</template>

<script>
var page = 2;
var temp = 1;
import uniSegmentedControl from '@/components/uni-segmented-control.vue';
import uniList from '@/components/uni-list/uni-list.vue';
import uniListItem from '@/components/uni-list-item/uni-list-item.vue';
import youScroll from '@/components/you-scroll';
import uCharts from '@/components/u-charts/u-charts.js';
import { isJSON } from '@/common/checker.js';
var _self;
var canvaLineA = null;

export default {
	components: {
		uniSegmentedControl,
		uniList,
		uniListItem,
		youScroll
	},
	data() {
		return {
			title: 'Hello',
			items: ['列表', '图表'],
			styleType: 'text',
			current: 0,
			activeColor: '#000000',
			//active-color:"blue",
			members: [],
			page: 1,
			data: [],
			loadtext: '上拉加载更多....',
			//listcolor:"red",
			//图标
			cWidth: '',
			cHeight: '',
			pixelRatio: 1,
			textarea: ''
		};
	},
	onLoad() {
		var loginRes=this.checkLogin('../index/index',2);
		uni.showLoading({
			title: '加载中...'
		});
		uni.request({
			url: 'https://www.auster.fun/doortp/public/api/door/blogs?token=api2020',
			method: 'GET',
			data: {},
			success: res => {
				uni.hideLoading();
				console.log(res.data.data);
				this.members = res.data.data;
			},
			fail: () => {},
			complete: () => {}
		});
		//图标onload
		_self = this;
		//#ifdef MP-ALIPAY
		uni.getSystemInfo({
			success: function(res) {
				if (res.pixelRatio > 1) {
					//正常这里给2就行，如果pixelRatio=3性能会降低一点
					//_self.pixelRatio =res.pixelRatio;
					_self.pixelRatio = 2;
				}
			}
		});
		//#endif
		this.cWidth = uni.upx2px(750);
		this.cHeight = uni.upx2px(500);
		this.getServerData();
	},
	methods: {
		totestPAGE(){
			uni.redirectTo({
				url: '../adduser/adduser',
				success: res => {},
				fail: () => {},
				complete: () => {}
			});
		},
		openDetail(e) {
			console.log('显示细节');
			console.log(e);
		},
		onClickItem(e) {
			if (this.current !== e.currentIndex) {
				this.current = e.currentIndex;
				if (e.currentIndex == 1) {
					this.getServerData();
					console.log('开始');
				}
			}
		},

		onPullDown(done) {
			// 下拉刷新
			uni.request({
				url: 'https://www.auster.fun/doortp/public/api/door/blogs?token=api2020',
				method: 'GET',
				data: {},
				success: res => {
					uni.hideLoading();
					console.log(res.data.data);
					this.members = res.data.data;
					done(); // 完成刷新
				},
				fail: () => {},
				complete: () => {}
			});
			//done(); // 完成刷新
		},
		onScroll(e) {
			// 监听滚动
			//console.log(e.detail.scrollTop);
		},
		onLoadMore(e) {
			//	console.log("加载更多ing...");
			console.log(temp);
			// 加载更多
			if (temp == 1) {
				temp = 0;
				// console.log('加载更很多~~');
				// page++;
				uni.request({
					url: 'https://www.auster.fun/doortp/public/api/door/blogs?token=api2020&page=' + page,
					method: 'GET',
					data: {},
					success: res => {
						uni.hideLoading();
						console.log('加载更多成功');
						console.log(res.data.data);
						if (res.data.data.length > 0) {
							for (let i = 0; i < 10; i++) {
								this.members.push(res.data.data[i]);
							}
						}
						// temp = 1;
						page++;
					},
					fail: () => {
						console.log('加载更多失败');
					},
					complete: () => {
						console.log('加载更多结束');
					}
				});
				temp = 1;
			} else return;
		},
		scrollTo() {
			this.$refs.scroll.goTop();
		},
		// 图方法

		getServerData() {
			uni.request({
				url: 'https://www.auster.fun/doortp/public/api/door/line?token=api2020',
				//url: 'https://www.ucharts.cn/data.json',
				data: {},
				success: function(res) {
					//console.log(res.data.data)
					let LineA = { categories: [], series: [] };
					//这里我后台返回的是数组，所以用等于，如果您后台返回的是单条数据，需要push进去
					LineA.categories = res.data.data.categories;
					LineA.series = res.data.data.series;
					console.log('开始打印');
					console.log(res.data.data.categories);
					console.log(res.data.data.series);
					console.log('综合');
					console.log(res.data.data);
					_self.textarea = JSON.stringify(res.data.data);
					_self.showLineA('canvasLineA', LineA);
				},
				fail: () => {
					_self.tips = '网络错误，小程序端请检查合法域名';
				}
			});
		},
		showLineA(canvasId, chartData) {
			canvaLineA = new uCharts({
				$this: _self,
				canvasId: canvasId,
				type: 'line',
				fontSize: 11,
				padding: [15, 15, 0, 15],
				legend: {
					show: true,
					padding: 5,
					lineHeight: 11,
					margin: 5
				},
				dataLabel: true,
				dataPointShape: true,
				dataPointShapeType: 'hollow',
				background: '#FFFFFF',
				pixelRatio: _self.pixelRatio,
				categories: chartData.categories,
				series: chartData.series,
				animation: true,
				enableScroll: true, //开启图表拖拽功能
				xAxis: {
					disableGrid: false,
					type: 'grid',
					gridType: 'dash',
					itemCount: 4,
					scrollShow: true,
					scrollAlign: 'left'
				},
				yAxis: {
					//disabled:true
					gridType: 'dash',
					splitNumber: 2,
					min: 10,
					max: 30,
					format: val => {
						return val.toFixed(0) + '人';
					} //如不写此方法，Y轴刻度默认保留两位小数
				},
				width: _self.cWidth * _self.pixelRatio,
				height: _self.cHeight * _self.pixelRatio,
				extra: {
					line: {
						type: 'straight'
					}
				}
			});
		},
		touchLineA(e) {
			canvaLineA.scrollStart(e);
		},
		moveLineA(e) {
			canvaLineA.scroll(e);
		},
		touchEndLineA(e) {
			canvaLineA.scrollEnd(e);
			//下面是toolTip事件，如果滚动后不需要显示，可不填写
			canvaLineA.touchLegend(e);
			canvaLineA.showToolTip(e, {
				format: function(item, category) {
					return category + ' ' + item.name + ':' + item.data;
				}
			});
		},
		changeData() {
			if (isJSON(_self.textarea)) {
				let newdata = JSON.parse(_self.textarea);
				canvaLineA.updateData({
					series: newdata.series,
					categories: newdata.categories,
					scrollPosition: 'right',
					animation: false
				});
			} else {
				uni.showToast({
					title: '数据格式错误',
					image: '../../../static/images/alert-warning.png'
				});
			}
		}
	}
};
</script>

<style scoped>
.uni-padding-wrap {
	margin: 0 auto;
	width: 75%;
	padding: 0px 0px;
}
.none {
	margin: auto auto;
}

page {
	height: 100%;
	overflow: hidden;
}

.content {
	/* text-align: center; */
	height: 100%;
}
.content .block {
	height: 200px;
	line-height: 200px;
	text-align: center;
	margin: 10px;
	background-color: aquamarine;
}
.content .loadingText {
	line-height: 30px;
	text-align: center;
	font-size: 12px;
	color: #999;
}

#preloader_1 {
	height: 40px;
	line-height: 40px;
	position: relative;
	text-align: center;
}
#preloader_1 span {
	width: 5px;
	height: 5px;
	margin: 0 1px;
	display: inline-block;
	vertical-align: middle;
	background: #9b59b6;
	animation: preloader_1 1.5s infinite ease-in-out;
}
#preloader_1 span:nth-child(2) {
	animation-delay: 0.2s;
}
#preloader_1 span:nth-child(3) {
	animation-delay: 0.4s;
}
#preloader_1 span:nth-child(4) {
	animation-delay: 0.6s;
}
#preloader_1 span:nth-child(5) {
	animation-delay: 0.8s;
}
@keyframes preloader_1 {
	0% {
		height: 5px;
		background: #9b59b6;
	}
	25% {
		height: 30px;
		background: #3498db;
	}
	50% {
		height: 5px;
		background: #9b59b6;
	}
	100% {
		height: 5px;
		background: #9b59b6;
	}
}
uni-list .uni-list-item {
	color: #ff0000;
}
/* 图样式 */
/*样式的width和height一定要与定义的cWidth和cHeight相对应*/
.qiun-charts {
	width: 750upx;
	height: 500upx;
	background-color: #ffffff;
}

.charts {
	width: 750upx;
	height: 500upx;
	background-color: #ffffff;
}
</style>
