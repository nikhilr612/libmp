/**
 * Copyright 2023 The MediaPipe Authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import {NormalizedKeypoint} from '../../../../tasks/web/components/containers/keypoint';

/**
 * The segmentation tasks return the segmentation either as a WebGLTexture (when
 * the output is on GPU) or as a typed JavaScript arrays for CPU-based
 * category or confidence masks. `Uint8ClampedArray`s are used to represent
 * CPU-based category masks and `Float32Array`s are used for CPU-based
 * confidence masks.
 */
export type SegmentationMask = Uint8ClampedArray|Float32Array|WebGLTexture;


/** A Region-Of-Interest (ROI) to represent a region within an image. */
export declare interface RegionOfInterest {
  /** The ROI in keypoint format. */
  keypoint: NormalizedKeypoint;
}

/** A connection between two landmarks. */
export declare interface Connection {
  start: number;
  end: number;
}
