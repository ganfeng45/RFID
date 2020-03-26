<template>
	<section><h1>hello world~</h1></section>
</template>
<script>
export default {
	data() {
		return {
			timer: '',
			value: 0
		};
	},
	methods: {
		get() {
			this.value ++;
			console.log(this.value);
			uni.request({
				url: 'https://www.auster.fun/doortp/public/api/door/blogs?token=api2020',
				method: 'GET',
				data: {},
				success: res => {
					console.log('成功');
					console.log(res);
					clearInterval(this.timer);
					console.log('定时器销毁');
				},
				fail: () => {},
				complete: () => {}
			});
		}
	},
	mounted() {
		this.timer = setInterval(this.get, 1000);
	},
	beforeDestroy() {
		clearInterval(this.timer);
	}
};
</script>
